#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include<fcntl.h>
#include<sys/wait.h>

#define READSIZE 6000000
void child(int);

/********************************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<sys/signal.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<string.h>
/**
 * [paramParser function to read input and parse the input from user to shell commands and connectors for commands like &&, || and ;]
 * @param  cnt [variable to store number of commnads and connectors input by user]
 * @return     [return 2D array containing user input shell commands and command connectors]
 */
char **paramParser( int *cnt, char argv[])
{
    char **lines;

    int cmd = 0, lineLen = 0, i = 0;
    int AND_FLAG = 0, OR_FLAG = 0;

    lines = malloc( 256 * sizeof(char *));
    for(int i = 0; i < 256; i++)
        lines[i] = malloc(4096 * sizeof(char));

    //char *input = strdup(line);
    char alph;
    int r = 0;
    while( r <= strlen(argv))
    {
        alph = argv[r++];

        if(alph == '\n')
        {
            if(lineLen != 0)
            {
                lines[cmd][lineLen] = '\0';
                cmd++;
                lineLen = 0;
            }
            break;
        }
        else if( alph == ' ')
        {
            if( lineLen != 0 )
            {
                lines[cmd][lineLen] = '\0';
                cmd++;
                lineLen = 0;
            }
        }
        else if( alph == '&')
        {
            if( AND_FLAG == 0)
            {
                AND_FLAG = 1;
                if( lineLen != 0 )
                {
                    lines[cmd][lineLen] = '\0';
                    cmd++;
                }
                lineLen = 0;
                strcpy(&lines[cmd][lineLen++], &alph);
            }
            else if( AND_FLAG == 1)
            {
                AND_FLAG = 0;
                strcpy(&lines[cmd][lineLen++], &alph);
                lines[cmd][lineLen] = '\0';
                cmd++;
                lineLen = 0;
            }

        }
        else if( alph == '|')
        {
            if( OR_FLAG == 0)
            {
                OR_FLAG = 1;
                if( lineLen != 0 )
                {
                    lines[cmd][lineLen] = '\0';
                    cmd++;
                }
                lineLen = 0;
                strcpy(&lines[cmd][lineLen++], &alph);
            }
            else if( OR_FLAG == 1)
            {
                OR_FLAG = 0;
                strcpy(&lines[cmd][lineLen++], &alph);
                lines[cmd][lineLen] = '\0';
                cmd++;
                lineLen = 0;
            }

        }
        else if( alph == ';' || alph == '>' )
        {
            if(lineLen != 0)
            {
                lines[cmd][lineLen] = '\0';
                cmd++;
                lineLen = 0;
            }
            strcpy(&lines[cmd][lineLen++], &alph);
            lines[cmd][lineLen] = '\0';
            cmd++;
            lineLen = 0;
        }
        else
            strcpy(&lines[cmd][lineLen++], &alph);


    }
    *cnt = cmd;
    if(cmd == 0)
        return NULL;

    return lines;
}
/**
 * [argListCreater create a 3D array for all user input shell commands with their arguments]
 * @param  lines [2D input array from which command argument 3D array is generated]
 * @param  cnt   [Number of values in lines 2D array]
 * @param  con   [2D array to store shell command connectors]
 * @return       [return 3D array which is NULL terminated to be passed to execvp to execute shell commands]
 */
char  ***argListCreater(char **lines, int cnt, char **con)
{
    char ***arg;
    if(cnt == 0)
    {
        con = NULL;
        return NULL;
    }

    // con = malloc(cnt * sizeof(char *));
    arg = malloc(cnt * sizeof(char **));
    for(int i = 0; i < cnt; i++)
    {
        //con[i] = malloc(cnt * sizeof(char));
        arg[i] = malloc(cnt * sizeof(char *));
        for( int j = 0; j < cnt; j++)
            arg[i][j] = malloc( 4096 * sizeof(char));
    }

    int cmd = 0, i = 0, j = 0, c = 0;
    while( cmd < cnt)
    {
        if( strcmp(lines[cmd], "&&") != 0 && strcmp(lines[cmd], "||") != 0
                && strcmp(lines[cmd], ";") != 0 && strcmp(lines[cmd], ">") != 0)
        {
            strcpy(arg[i][j++], lines[cmd]);
        }
        else
        {
            arg[i][j] = NULL;
            i++;
            j = 0;
            strcpy(con[c], lines[cmd]);
            c++;
        }
        cmd++;
    }
    arg[i][j] = NULL;
    arg[++i] = NULL;
    con[c] = NULL;
    //connectors=con;
    /*printf("Commands: %d\n",i );
    j = 0;
    for(int x = 0; x <= i; x++)
    { j=0;
        while( arg[x][j] != NULL)
        {
            printf("%s ", arg[x][j++]);
        }
        printf("\n");
    }*/

    return arg;
}



int exc(char *argv)
{





    int cmd = 0;

    int con = 0, cnt = 0;

    //printf("\nEnter shell Command\n");
    // scanf("%[^\n]%*c", line);
    char **lines = paramParser( &cmd, argv);

    // printf("%d\n", cmd);

    // if(lines != NULL)
    //     for(int i = 0; i < cmd; i++)
    //         printf("%s\nlength=%ld\n", lines[i], strlen(lines[i]));

    char **connectors;
    connectors = malloc(cmd * sizeof(char *));
    for(int i = 0; i < cmd; i++)
        connectors[i] = malloc(cmd * sizeof(char));

    char  ***arg = argListCreater(lines, cmd, connectors);

    int i = 0;

    if(arg != NULL)
        while(arg[i] != NULL )
        {
            // int j = 0;
            // while( arg[i][j] != NULL )
            // {
            //   //printf("%s ", arg[i][j++]);
            // }

            // printf("\n");
            i++;
            cnt++;
        }
    //printf("%d\n", i);

    i = 0;
    while(connectors[i] != NULL && arg != NULL)
    {
        //printf("here\n");
        //printf("%s\n", connectors[i++] );
        i++;
        con++;
    }


    if(lines != NULL)
        free(lines);

    //printf("cmds: %d cons: %d \n", cnt, con );

    // if(cnt == 0)
    // {
    //     if(arg != NULL)
    //         free(arg);
    //     free(connectors);
    //     //printf("here\n");
    //     //continue;
    // }

    int *extCodes = malloc(cnt * sizeof(int ));
    int x = 0, flag = 0;
    for(i = 0; i < cnt; i++)
    {
        if(i != 0)
        {
            if( x < con)
                if(strcmp( connectors[x], "&&") == 0 )
                {
                    //printf("Connectors index %d\n",x );
                    if(extCodes[i - 1] == 0)
                    {
                        flag = 0;
                        //extCodes[i+1]=0;
                    }
                    else
                    {
                        flag = 1;
                    }
                    x++;

                }
                else if(strcmp( connectors[x], "||") == 0)
                {
                    //printf("Connectors index %d\n",x );
                    if(extCodes[i - 1] == 0)
                    {
                        flag = 1;
                        extCodes[i] = 0;
                        //i++;
                    }
                    else
                    {
                        flag = 0;
                    }
                    x++;
                }
                else if( strcmp( connectors[x], ";") == 0)
                {
                    //printf("Connectors index %d\n",x );
                    flag = 0;
                    //extCodes[i+1]=0;
                    x++;
                }
        }
        if( flag == 0)
            if(fork() == 0)
            {
                //printf("Executing %s %d\n", arg[i][0], i );
                int rst = execvp(arg[i][0], arg[i]);
                if(rst == -1)
                {
                    printf("%s: Command not found!\n", arg[i][0]);
                }
                exit(rst);

            }
            else
            {
                int n;
                wait(&n);
                if(WEXITSTATUS(n) == 0)
                    extCodes[i] = 0;
                else
                    extCodes[i] = -1;

            }
    }



    // for(i = 0 ; i < cnt; i++)
    //     printf("%d\n", extCodes[i] );

    if(arg != NULL)
        free(arg);
    free(connectors);
    free(extCodes);



}






/********************************************************************************************/









int main(int argc, char *argv[])
{
    int sd, client, portNumber;
    struct sockaddr_in servAdd;     // server socket address

    if(argc != 2)
    {
        printf("Call model: %s <Port Number>\n", argv[0]);
        exit(0);
    }

    sd = socket(AF_INET, SOCK_STREAM, 0);
    servAdd.sin_family = AF_INET;
    servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
    sscanf(argv[1], "%d", &portNumber);
    servAdd.sin_port = htons((uint16_t)portNumber);

    bind(sd, (struct sockaddr *) &servAdd, sizeof(servAdd));
    listen(sd, 5);

    while(1)
    {
        client = accept(sd, NULL, NULL);
        printf("Got a client\n");

        if(!fork())
            child(client);

        close(client);
    }
}

void child(int sd)
{
    int STDIN, STDOUT, STDERR;
    dup2(0, STDIN);
    dup2(1, STDOUT);
    dup2(3, STDERR);
    char message[255];


    while(1)
    {
        fprintf(stdout, "here\n");
        //fgets(message, 254, stdin);
        write(sd, "\n(null):~$ \0", 12);

        if(!read(sd, message, 255))
        {
            close(sd);
            fprintf(stderr, "Bye, my client is dead, wait for a new one\n");
            exit(0);
        }
        fprintf(stderr, "Client send back: %s\n", message);

        int pid_exc = fork();

        if(pid_exc)               //parent waiting
        {
            int status;
            wait(&status);
            printf("exc %d\n", WEXITSTATUS(status));
        }
        else                      //Child executing
        {
            int stderr_clntPipe[2];

            if (pipe(stderr_clntPipe) != 0)
                perror("stderr_clntPipe pipe() failed");

            dup2(stderr_clntPipe[1], 2);
            int stdwr_child = fork();
            if(stdwr_child)       //parent
            {
                //close(stdout_clntPipe[0]);
                //close(stderr_clntPipe[1]);
                close(stderr_clntPipe[0]);
                //fprintf(stderr, "%s\n", message );
                dup2(sd, 1);
                exc(message);
                close(stderr_clntPipe[1]);
                //int ll;
                //wait(&ll);
                sleep(1);
                //close(stdout_clntPipe[1]);
                dup2(STDOUT_FILENO, 1);
                dup2(STDERR_FILENO, 2);
                //printf("stdwr_child %d\n",WEXITSTATUS(ll) );
                //fprintf(stderr, "stdwr_child %d\n", WEXITSTATUS(ll));
                exit(4);
            }
            else                //child
            {
                
                close(stderr_clntPipe[1]);
                //close(stderr_clntPipe[0]);
                
                char *msg = malloc(READSIZE * sizeof(char));
                if (read(stderr_clntPipe[0], msg, READSIZE) < 0)
                {
                    fprintf(stderr, "read() error\n");
                    exit(3);
                }
                close(stderr_clntPipe[0]);
                dup2(STDERR_FILENO, 2);
                write(sd, msg, strlen(msg));
                fprintf(stderr, "%ld done\n", strlen(msg));
                exit(0);
            }
        }

    }
}
