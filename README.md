# FALA-FALA-IRC
## SSC0142 - Redes de Computadores 
*1 Semestre - 2022*

### Integrantes:
- Enrique Gabriel da Silva Teles
- Luiz Gustavo Sincaruk Vieira
- Maurílio da Motta Meireles

### Como executar:
Para executar o projeto é necessário compilar os códigos fontes utilizando o commando `make` e após deve rodar os executáveis em terminais diferentes.

Primeiro executamos o *server*: `./server`; e logo após executamos o *client*: `./client`. Com isso podemos enviar e receber mensagens através da conexão entre eles de forma assíncrona, ou seja, não precisamos esperar receber uma mensagem para enviar outra e nem enviar para poder receber.

Após a utilização, se quiser, pode-se limpar os arquivos gerados com o comando `make clean`.

### Ambiente utilizado
Para o desenvolvimento e execução foi usado:
```
Sistema Operacional: Pop!_OS 21.10
Kernel: 5.17.5
Compilador: GCC (Ubuntu 11.2.0-7ubuntu2) 11.2.0
```