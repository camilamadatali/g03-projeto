###### Camila Madatali nº 20210832
###### Gonçalo Alves nº 20210544
###### Tatiana Yermachenkava nº 20210697

## Universidade Europeia, IADE - Curso de Engenharia Informática

### Trabalho realizado no âmbito da Unidade Curricular de Sistemas Operativos



# Descrição do Problema

###### 
O projeto visa desenvolver um interpretador de comandos para o sistema operativo Linux, incorporando características inovadoras para a melhor eficiência e a usabilidade. A motivação para este trabalho encontra-se em possibilitar aos utilizadores uma experiência mais intuitiva e eficaz no uso do terminal, estabelecendo uma ligação entre comandos internos e externos de forma dinâmica.  

## Casos de Uso

###### 
Um Interpretador de Comandos para criptografar arquivos:
Uma clínica armazena informações confidenciais sobre a saúde dos seus pacientes. Para proteger esses dados sensíveis contra acessos não autorizados e garantir a privacidade dos pacientes, a clínica implementou um sistema de criptografia e descriptografia de arquivos. Os médicos e enfermeiros da clínica geram e armazenam arquivos digitais contendo esses dados. Esses arquivos devem ser criptografados antes de serem armazenados nos servidores da clínica. Quando necessário, os arquivos criptografados são descriptografados para serem acedidos pelo pessoal autorizado para revisão e tratamento de dados. 

Um Interpretador de Comandos para Consulta de Condições Meteorológicas:
A Joana é uma programadora que trabalha remotamente para uma empresa internacional. Um dia, Joana estava calmamente a trabalhar no seu terminal do ubuntu, até que recebe uma chamada do chefe dela e descobre que têm de fazer uma viagem de trabalho urgente para Berlim. Como já se encontrava no terminal do ubuntu e não tinha tempo a perder para se preparar para esta viagem, vai rapidamente ao terminal consultar a meteorologia de Berlim, e começa logo a fazer a sua mala.  

Um Interpretador de Comandos para Gestão de Tarefas:
Luís é um aluno de engenharia informática no seu último semestre, e está a sentir-se atrapalhado com a quantidade de projetos, frequências e apresentações que têm de fazer em pouco tempo. E um dos projetos dele é no terminal do ubuntu, por isso como sabe que vai passar muito tempo no terminal, decide gerir as suas tarefas lá, e assim consegue estar mais organizado e ciente da quantidade de trabalho que tem para fazer. 

# Alterações Realizadas
Em relação à gestão de tarefas foi retirada a funcionalidade dos lembretes, e adicionada as de completar e remover tarefas. Além disso, foi implementado a persistência dos dados para que o utilizador possa visualizar as suas tarefas mesmo se sair do programa. 
O resto das funções não sofreram quaisquer alterações e foram concluídas tal como esperado. 

# Descrição da Solução Implementar

######
Encrypt File / Decrypt File: Ambas as funções EncryptFile e DecryptFile operam de maneira semelhante. Primeiro, começam por abrir um arquivo de entrada para leitura e um arquivo de saída para escrita. De seguida, um contexto de criptografia é inicializado com o algoritmo AES em modo ECB e a chave fornecida. 

Os dados do arquivo de entrada são lidos em blocos e processados pela função EVP_EncryptUpdate na EncryptFile e EVP_DecryptUpdate na DecryptFile. Essa função criptografa ou descriptografa os dados e armazena-os num buffer de saída. 

Após o processamento de todos os blocos de dados, as funções EVP_EncryptFinal_ex ou EVP_DecryptFinal_ex são chamadas para finalizar o processo de criptografia ou descriptografia. Qualquer dado final que possa ter sido armazenado no contexto de criptografia é processado e criptografado ou descriptografado. 

Por fim, os dados processados são escritos no arquivo de saída e os recursos são libertados, incluindo o contexto de criptografia e o fecho dos arquivos de entrada e saída. 

As funções EncryptFile e DecryptFile têm vantagens e desvantagens muito semelhantes. Estas funções oferecem vantagens como a simplicidade de implementação, eficiência de processamento ao operar em blocos de dados moderados e flexibilidade para criptografar e descriptografar arquivos. Por outro lado, enfrentam limitações de segurança devido ao uso do modo ECB, que é vulnerável a certos tipos de ataques, como ataques de repetição. A gestão adequada das chaves também é crucial para garantir a segurança dos dados criptografados. 

Gestão de Tarefas: A estrutura de dados ‘Task’ é definida para representar cada tarefa, armazena as informações da descrição e estado de cada tarefa. A função ‘addTask’ adiciona uma nova tarefa à lista de tarefas, e verifica se o limite máximo de tarefas foi atingido antes de adicionar uma nova. A função ‘listTasks’ lista todas as tarefas existentes, mostrando o seu índice, estado e descrição. A função ‘completeTask’ mara uma tarefa especifica como concluída com base no índice fornecido. A função ‘removeTask’ remove a tarefa com base no índice fornecido, movendo todas as tarefas restantes uma posição para cima. A função ‘saveTasksFile’ é executada para as tarefas do utilizador serem guardadas num arquivo mesmo depois do utilizador sair do programa. E por fim, a ‘loadTaskFromFile’ é responsável por carregar as tarefas do arquivo guardado.  

Esta implementação é uma boa base para uma aplicação de gestão de tarefas, sendo simples e funcional para as necessidades básicas de um utilizador, tendo baixa dependência de bibliotecas externas e persistência de dados. Mas, para torna esta implementação mais adequada para um público mais amplo e exigente, seria necessário melhorara a escalabilidade, seguranças, interface e adicionar funcionalidades com prazos e prioridades às tarefas. 

Meteorologia: A função GetWeather é usada para obter informações climáticas de uma cidade específica. Ela constrói um URL com base no nome da cidade fornecida e na API_KEY, utilizando a função snprintf para garantir a segurança e evitar problemas de buffer overflow. Em seguida, a função faz uma solicitação HTTP para a URL construída utilizando a biblioteca cURL. 

A função WriteCallback é a função de retorno de chamada configurada pelo cURL para processar os dados recebidos. Ela é responsável por analisar os dados JSON utilizando a biblioteca cJSON, extraindo informações relevantes sobre o clima, como a descrição, temperatura, humidade e velocidade do vento. Esses dados são então formatados e exibidos ao utilizador. 

Construção da URL: A URL é construída dinamicamente utilizando a função snprintf, que permite incorporar de forma segura o nome da cidade e a key da API na string da URL. 

Solicitação HTTP: A solicitação HTTP é realizada através da função curl_easy_perform, configurada com a URL e a função de retorno de chamada WriteCallback para processar os dados recebidos. 

Processamento dos Dados JSON: A função WriteCallback processa os dados JSON recebidos da resposta HTTP. Ela utiliza a biblioteca cJSON para analisar os dados e extrair informações específicas sobre o clima. Por fim estas informações são então apresentadas ao utilizador. 

## Descrição Genérica

######
Com este projeto desenvolvemos um interpretador de comandos para o ambiente Linux, especificamente para o terminal do Ubuntu. O interpretador permite ao utilizador executar os comandos básicos, bem como comandos adicionais diretamente no terminal, de forma eficiente e intuitiva para a interação com o sistema operativo.

## Enquadramento nas áreas da Unidade Curricular

######
Este projeto envolve a criação de um Shell para Linux enquadra-se nas áreas curriculares de Sistemas Operativos através do gerenciamento de processos, manipulação de arquivos, comunicação entre processos, gerenciamento de tarefas e memória, interação do utilizador com o sistema, e integração de bibliotecas externas.

## Requisitos Técnicos para o desenvolvimento do projeto 

######
- Interpretador de Comandos: O programa deve ser capaz de interpretar comandos inseridos pelos utilizadores no terminal. 
- Funcionalidades Específicas: Deve suportar funcionalidades, como consultar o clima, criptografar e descriptografar arquivos e gerenciar uma lista de tarefas. 
- Interatividade: Deve permitir interações contínuas com o utilizador, fornecendo um prompt no terminal e processar os dados inseridos em tempo real.  
- Tratamento de Erros: Deve ser capaz de lidar com erros de entrada do utilizador. 

## Arquitetura de Solução 

######
O programa é implementado em linguagem C, estruturado em torno de um loop infinito, onde espera pelos comandos inseridos pelo utilizador. A entrada dos comandos é feita através da função ‘DisplayPrompt( )’, que exibe um prompt no terminal do Ubuntu, indicado que está pronto para receber dados. E o comando digitado pelo utilizador é lido através da função ‘fgets( )’ e é armazenado numa variável ‘input’.  

Após a leitura dos comandos, o programa processa os, e verifica se correspondem a comandos internos, como a mudança de diretório, operações de criptografia, consulta da meteorologia, e gestão de tarefas.   

Por fim, quando o utilizador digita ‘exit’, as tarefas do momento são guardadas e o arquivo shell é encerrado. 

## Tecnologias a utilizar

#####
- Linguagem de Programação C: Utilizada para desenvolver a lógica principal do interpretador de comandos. 
- Bibliotecas Padrão C:’ <stdio.h>’,’ <stdlib.h>’,’ <string.h>’,’ <unistd.h>’, para funcionalidades básicas de entrada e saída, manipulação de strings, etc. 
- OpenSSL: Utilizado para criptografar e descriptografar arquivos utilizando o algoritmo AES (Advanced Encryption Standard).  
- cURL: Utilizado para realizar requisições HTTP e obter dados da API da previsão do tempo. 
- cJSON: Utilizado para manipulação de dados JSON retornados pela API da meteorologia. 

# Planeamento e Calendarização

######


# Resultados
Neste projeto, foi possível a implementação eficaz do shell personalizado para o ambiente Linux, fornecendo aos utilizadores uma interface mais intuitiva para interagir com o sistema operacional. As funcionalidas adicionais como a criptografia de arquivos, consulta da meteorologia e gestão de tarefas ampliam a utilidade do shell. 
Pontos fortes da implementação:  
- Funcionalidades adicionais. 
- Persistência de dados. 
- Integração de bibliotecas externas. 
- Segurança na construção de URL. 

Pontos fracos da implementação: 
- enfrentam limitações de segurança devido ao uso do modo ECB 
# Bibliografia 

######
Install, L. P. (2016, January 27). Shell, Bash e Scripts: Tudo sobre Shell Scripting. Desde Linux. https://blog.desdelinux.net/pt/shell-bash-e-scripts-sobre-scripts-de-shell/
Comandos do bash: ls, head, mv e cat explicados e com exemplos. (2023, November 14). FreeCodeCamp.org. https://www.freecodecamp.org/portuguese/news/comandos-do-bash-ls-head-mv-e-cat-explicados-e-com-exemplos/

Weather from terminal. (n.d.). Ask Ubuntu. Retrieved March 2, 2024, from https://askubuntu.com/questions/390329/weather-from-terminal

OpenAI. (2022, November 30). ChatGPT. Chat.openai.com; OpenAI. https://chat.openai.com/

Jaydatt. (2024, February 21). jaydattpatel/Linux-commands-and-Shell-Scripts. GitHub. https://github.com/jaydattpatel/Linux-commands-and-Shell-Scripts

Saive, R. (2022, September 12). 6 Best To-Do List Managers for Linux Command Line. Www.ubuntumint.com. https://www.ubuntumint.com/command-line-to-do-list-managers/

Dancuk, M. (2023, November 16). SCP Command in Linux {13 Examples} | phoenixNAP KB. Knowledge Base by PhoenixNAP. https://phoenixnap.com/kb/linux-scp-command

Let’s Build a Linux Shell [Part I] | HackerNoon. (n.d.). Hackernoon.com. Retrieved March 3, 2024, from https://hackernoon.com/lets-build-a-linux-shell-part-i-bz3n3vg1

Developing a Linux based shell. (2018, June 18). GeeksforGeeks. https://www.geeksforgeeks.org/developing-linux-based-shell/

Openssl EVP encryption and decryption from a file. (n.d.). Stack Overflow. Retrieved May 26, 2024, from https://stackoverflow.com/questions/44635285/openssl-evp-encryption-and-decryption-from-a-file

EVP Symmetric Encryption and Decryption - OpenSSLWiki. (n.d.). Wiki.openssl.org. https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption

‌How to decrypt using openssl EVP? (n.d.). Stack Overflow. Retrieved May 26, 2024, from https://stackoverflow.com/questions/65161156/how-to-decrypt-using-openssl-evp

‌Openssl EVP encryption and decryption from a file. (n.d.). Stack Overflow. Retrieved May 26, 2024, from https://stackoverflow.com/questions/44635285/openssl-evp-encryption-and-decryption-from-a-file

‌https://stackoverflow.com/questions/14911156/how-to-properly-reuse-a-curl-handle

https://stackoverflow.com/questions/45709054/create-json-object-using-cjson-h

OpenWeatherMap.org. (2015). Weather API - OpenWeatherMap. Openweathermap.org. https://openweathermap.org/api
C Saving Data from a Text File into an Array of Structures. (n.d.). Stack Overflow. https://stackoverflow.com/questions/50240192/c-saving-data-from-a-text-file-into-an-array-of-structures
C library function - fopen(). (n.d.). Www.tutorialspoint.com. https://www.tutorialspoint.com/c_standard_library/c_function_fopen.htm


‌




