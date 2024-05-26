#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <curl/curl.h> 
#include "cJSON.h"

#define INPUT_SIZE 510
#define KEY_SIZE 16 
#define API_KEY "b7b3289c1f8d76c812aeee199fe7a78c" 
#define MAX_TASKS 100
#define MAX_TASK_LENGTH 50
#define TASKS_FILENAME "tasks.txt"

typedef struct {
    char description[MAX_TASK_LENGTH];
    int completed;
} Task;

Task taskList[MAX_TASKS];
int taskCount = 0;

void DisplayPrompt();
char** ParseInput(char *input);
void FreeMemory(char** argv);
void MyChangeDirectory(char* path);
void EncryptFile(const char* input_filename, const char* output_filename, const unsigned char* key);
void DecryptFile(const char* input_filename, const char* output_filename, const unsigned char* key);
void GetWeather(const char* city);
void MyCJSON_Delete(cJSON *item);
void addTask(const char *description);
void listTasks();
void completeTask(int index);
void removeTask(int index);
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp); 
void saveTasksToFile(const char *filename);
void loadTasksFromFile(const char *filename);

int main() {
    loadTasksFromFile(TASKS_FILENAME);

    char input[INPUT_SIZE] = "";
    unsigned char key[KEY_SIZE + 1] = "";

    while (1) {
        DisplayPrompt();

        if (fgets(input, INPUT_SIZE, stdin) == NULL) {
            printf("fgets failed\n");
            continue;
        }

        input[strcspn(input, "\n")] = '\0'; 

        if (strcmp(input, "exit") == 0) {
            saveTasksToFile(TASKS_FILENAME);
            break;
        }
        else if (strncmp(input, "weather ", 8) == 0) {
            char city[INPUT_SIZE - 8];
            strcpy(city, input + 8);
            if (strlen(city) > 0) {
                GetWeather(city);
            } else {
                printf("Erro: Nenhuma cidade fornecida.\n");
            }
        }
        else if (strncmp(input, "encrypt ", 8) == 0) {
            char input_filename[INPUT_SIZE], output_filename[INPUT_SIZE], key_string[KEY_SIZE + 1];
            if (sscanf(input, "encrypt %s %s %s", input_filename, output_filename, key_string) == 3) {
                EncryptFile(input_filename, output_filename, (unsigned char *)key_string);
            } else {
                printf("Erro: Comando inválido. Uso: encrypt <arquivo_entrada> <arquivo_saida> <chave>\n");
            }
        }
        else if (strncmp(input, "decrypt ", 8) == 0) {
            char input_filename[INPUT_SIZE], output_filename[INPUT_SIZE], key_string[KEY_SIZE + 1];
            if (sscanf(input, "decrypt %s %s %s", input_filename, output_filename, key_string) == 3) {
                DecryptFile(input_filename, output_filename, (unsigned char *)key_string);
            } else {
                printf("Erro: Comando inválido. Uso: decrypt <arquivo_entrada> <arquivo_saida> <chave>\n");
            }
        }
        else if (strncmp(input, "addtask ", 8) == 0) {
            char description[MAX_TASK_LENGTH];
            strcpy(description, input + 8);
            if (strlen(description) > 0) {
                addTask(description);
            } else {
                printf("Erro: Descrição da tarefa não fornecida.\n");
            }
        }
        else if (strcmp(input, "listtasks") == 0) {
            listTasks();
        }
        else if (strncmp(input, "completetask ", 13) == 0) {
            int index;
            if (sscanf(input, "completetask %d", &index) == 1) {
                completeTask(index);
            } else {
                printf("Erro: Índice inválido.\n");
            }
        }
        else if (strncmp(input, "removetask ", 11) == 0) {
            int index;
            if (sscanf(input, "removetask %d", &index) == 1) {
                removeTask(index);
            } else {
                printf("Erro: Índice inválido.\n");
            }
        }
        else {
            char** argv = ParseInput(input);

            if (strcmp(argv[0], "cd") == 0) {
                if (argv[1] == NULL) {
                    struct passwd *pw = getpwuid(getuid());
                    if (pw != NULL)
                        MyChangeDirectory(pw->pw_dir);
                } else {
                    MyChangeDirectory(argv[1]);
                }
            } else {
                pid_t id = fork();
                if (id < 0) {
                    printf("fork failed\n");
                    exit(EXIT_FAILURE);
                }
                else if (id == 0) {
                    execvp(argv[0], argv);
                    perror("execvp");
                    exit(EXIT_FAILURE);
                }
                else {
                    wait(NULL);
                }
            }

            FreeMemory(argv);
        }
    }
    return 0;
}

void DisplayPrompt() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        char *user = getlogin();
        if (user != NULL)
            printf("%s@%s$ ", user, cwd);
        else
            printf("%s$ ", cwd);
    } else {
        perror("getcwd");
    }
}

char** ParseInput(char *input) {
    char** argv = malloc(INPUT_SIZE * sizeof(char*));
    if (argv == NULL) {
        printf("malloc failed\n");
        exit(EXIT_FAILURE);
    }
    int i = 0;
    char* token = strtok(input, " ");
    while (token != NULL) {
        argv[i++] = strdup(token);
        token = strtok(NULL, " ");
    }
    argv[i] = NULL;

    return argv;
}

void FreeMemory(char** argv) {
    for (int i = 0; argv[i] != NULL; i++) {
        free(argv[i]);
    }
    free(argv); 
}

void MyChangeDirectory(char* path) {
    if (chdir(path) != 0) {
        perror("chdir");
    }
}
//https://stackoverflow.com/questions/44635285/openssl-evp-encryption-and-decryption-from-a-file
//https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
void EncryptFile(const char* input_filename, const char* output_filename, const unsigned char* key) {
    FILE *input_file = fopen(input_filename, "rb");
    if (!input_file) {
        perror("fopen");
        return;
    }

    FILE *output_file = fopen(output_filename, "wb");
    if (!output_file) {
        perror("fopen");
        fclose(input_file);
        return;
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL);

    unsigned char in_buf[1024], out_buf[1024 + EVP_MAX_BLOCK_LENGTH];
    int num_bytes_read, num_bytes_written;
    while ((num_bytes_read = fread(in_buf, 1, sizeof(in_buf), input_file)) > 0) {
        EVP_EncryptUpdate(ctx, out_buf, &num_bytes_written, in_buf, num_bytes_read);
        fwrite(out_buf, 1, num_bytes_written, output_file);
    }

    EVP_EncryptFinal_ex(ctx, out_buf, &num_bytes_written);
    fwrite(out_buf, 1, num_bytes_written, output_file);

    EVP_CIPHER_CTX_free(ctx);
    fclose(input_file);
    fclose(output_file);
}
//https://stackoverflow.com/questions/65161156/how-to-decrypt-using-openssl-evp
////https://stackoverflow.com/questions/44635285/openssl-evp-encryption-and-decryption-from-a-file
void DecryptFile(const char* input_filename, const char* output_filename, const unsigned char* key) {
    FILE *input_file = fopen(input_filename, "rb");
    if (!input_file) {
        perror("fopen");
        return;
    }

    FILE *output_file = fopen(output_filename, "wb");
    if (!output_file) {
        perror("fopen");
        fclose(input_file);
        return;
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        perror("EVP_CIPHER_CTX_new");
        fclose(input_file);
        fclose(output_file);
        return;
    }

    if (EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL) != 1) {
        perror("EVP_DecryptInit_ex");
        EVP_CIPHER_CTX_free(ctx);
        fclose(input_file);
        fclose(output_file);
        return;
    }

    unsigned char in_buf[1024 + EVP_MAX_BLOCK_LENGTH], out_buf[1024];
    int num_bytes_read, num_bytes_written;
    while ((num_bytes_read = fread(in_buf, 1, sizeof(in_buf), input_file)) > 0) {
        if (EVP_DecryptUpdate(ctx, out_buf, &num_bytes_written, in_buf, num_bytes_read) != 1) {
            perror("EVP_DecryptUpdate");
            EVP_CIPHER_CTX_free(ctx);
            fclose(input_file);
            fclose(output_file);
            return;
        }
        fwrite(out_buf, 1, num_bytes_written, output_file);
    }

    if (!feof(input_file)) {
        perror("fread");
        EVP_CIPHER_CTX_free(ctx);
        fclose(input_file);
        fclose(output_file);
        return;
    }

     if (EVP_DecryptFinal_ex(ctx, out_buf, &num_bytes_written) != 1) {
        if (errno == EINVAL) {
            fprintf(stderr, "Erro: Chave de descriptografia incorreta.\n");
        }
        EVP_CIPHER_CTX_free(ctx);
        fclose(input_file);
        fclose(output_file);
        return;
    }
    fwrite(out_buf, 1, num_bytes_written, output_file);

    EVP_CIPHER_CTX_free(ctx);
    fclose(input_file);
    fclose(output_file);
}

//https://stackoverflow.com/questions/14911156/how-to-properly-reuse-a-curl-handle
void GetWeather(const char* city) {
    CURL *curl;
    CURLcode res;
    char url[200]; 
    snprintf(url, sizeof(url), "http://api.openweathermap.org/data/2.5/weather?q=%s&appid=%s", city, API_KEY); 

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, city); 
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        curl_easy_cleanup(curl);
    }
}
//https://stackoverflow.com/questions/45709054/create-json-object-using-cjson-h
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    char *data = (char *)contents;
    char *city = (char *)userp; 

    cJSON *json = cJSON_Parse(data);
    if (!json) {
        fprintf(stderr, "Failed to parse JSON data\n");
        return realsize;
    }

    cJSON *weather = cJSON_GetObjectItem(json, "weather");
    cJSON *main = cJSON_GetObjectItem(json, "main");
    cJSON *wind = cJSON_GetObjectItem(json, "wind");
    cJSON *sys = cJSON_GetObjectItem(json, "sys");

    if (!weather || !main || !wind || !sys) {
        fprintf(stderr, "Missing required JSON fields\n");
        cJSON_Delete(json); 
        return realsize;
    }

    printf("Current Weather in %s:\n", city); 

    if (cJSON_IsArray(weather) && cJSON_GetArraySize(weather) > 0) {

        cJSON *first_weather_item = cJSON_GetArrayItem(weather, 0);

        cJSON *weather_description = cJSON_GetObjectItem(first_weather_item, "description");
        if (weather_description) {
            printf("Description: %s\n", weather_description->valuestring);
        } else {
            fprintf(stderr, "Weather description not found\n");
        }
    } else {
        fprintf(stderr, "Weather array not found or empty\n");
    }

    cJSON *temperature = cJSON_GetObjectItem(main, "temp");
    if (temperature) {
        printf("Temperature: %.2f°C\n", temperature->valuedouble - 273.15);
    } else {
        fprintf(stderr, "Temperature not found\n");
    }

    cJSON *humidity = cJSON_GetObjectItem(main, "humidity");
    if (humidity) {
        printf("Humidity: %d%%\n", humidity->valueint);
    } else {
        fprintf(stderr, "Humidity not found\n");
    }

    cJSON *wind_speed = cJSON_GetObjectItem(wind, "speed");
    if (wind_speed) {
        printf("Wind Speed: %.2f m/s\n", wind_speed->valuedouble);
    } else {
        fprintf(stderr, "Wind speed not found\n");
    }
    cJSON_Delete(json); 
    return realsize;
}

void addTask(const char *description) {
    if (taskCount < MAX_TASKS) {
        strcpy(taskList[taskCount].description, description);
        taskList[taskCount].completed = 0;
        taskCount++;
        printf("Tarefa adicionada com sucesso!\n");
    } else {
        printf("Limite máximo de tarefas atingido!\n");
    }
}

void listTasks() {
    printf("Lista de Tarefas:\n");
    for (int i = 0; i < taskCount; i++) {
        printf("%d. [%s] %s\n", i + 1, taskList[i].completed ? "X" : " ", taskList[i].description);
    }
}

void completeTask(int index) {
    if (index >= 1 && index <= taskCount) {
        taskList[index - 1].completed = 1;
        printf("Tarefa completada!\n");
    } else {
        printf("Índice inválido!\n");
    }
}

void removeTask(int index) {
    if (index >= 1 && index <= taskCount) {
        for (int i = index - 1; i < taskCount - 1; i++) {
            strcpy(taskList[i].description, taskList[i + 1].description);
            taskList[i].completed = taskList[i + 1].completed;
        }
        taskCount--;
        printf("Tarefa removida com sucesso!\n");
    } else {
        printf("Índice inválido!\n");
    }
}
//https://stackoverflow.com/questions/50240192/c-saving-data-from-a-text-file-into-an-array-of-structures
void saveTasksToFile(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file for writing");
        return;
    }

    for (int i = 0; i < taskCount; i++) {
        fprintf(file, "%d,%s\n", taskList[i].completed, taskList[i].description);
    }

    fclose(file);
}
//https://www.tutorialspoint.com/c_standard_library/c_function_fopen.htm
void loadTasksFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file for reading");
        return;
    }

    char line[INPUT_SIZE];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        char *token = strtok(line, ",");
        if (token != NULL) {
            taskList[taskCount].completed = atoi(token);
            token = strtok(NULL, ",");
            if (token != NULL) {
                strcpy(taskList[taskCount].description, token);
                taskCount++;
            }
        }
    }

    fclose(file);
}
