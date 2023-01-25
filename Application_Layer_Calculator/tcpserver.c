/* 
 * Echo server using TCP
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <math.h>

#define SERVER_TCP_PORT	3000
#define BUFLEN	256
#define MAXDIGITS 15
#define TOTALEXECUTIONS 8

int power(int base, int exponent);

int main(int argc, char **argv)
{

  int i, n, bytes_to_read;
  int sd, new_sd, client_len, port, hostname;
  struct sockaddr_in server, client;
  char* client_port;
  char *bp, buf[BUFLEN], tbuf[BUFLEN]="hello";
  char hostbuf[256], *IPbuf;
  struct hostent *host;
  
  switch(argc) {
  case 1:
    port = SERVER_TCP_PORT;
    break;
  case 2:
    port = atoi(argv[1]);
    break;
  default:
    fprintf(stderr, "Usage: %s [port]\n", argv[0]);
    exit(1);
  }
  
  /* Create a stream socket */
  
  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    fprintf(stderr, "ERROR: Can't create a socket\n");
    exit(1);
  }
  
  /* Bind an address to the socket */
  
  bzero((char *)&server, sizeof(struct sockaddr_in));
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  if(bind(sd, (struct sockaddr *)&server, sizeof(server)) == -1) {
    fprintf(stderr, "ERROR: Can't bind name to socket\n");
    exit(1);
  }

 /* Get host name, etc. from socket structure */

  if(gethostname(hostbuf, sizeof(hostbuf)) == -1) {
    fprintf(stderr, "ERROR: Problem with hostname\n");
    exit(1);
  }
  host = gethostbyname(hostbuf);
  if (host==NULL) {
   fprintf(stderr, "ERROR: Problem with hostname\n");
    exit(1);
  }
  IPbuf = inet_ntoa(*((struct in_addr*) host->h_addr_list[0]));
  printf("\n");
  printf("Server listening ... \n");
  printf("\t host: %s\n", hostbuf); 
  printf("\t IP: %s\n", IPbuf);
  printf("\t port: %d\n\n", port);

  /* Queue connect requests */
  
  listen(sd,5);
  int inputData[BUFLEN-1];
  int executionCounter = 0;
  
  client_len = sizeof(client);
  if ((new_sd = accept(sd, (struct sockaddr *)&client, &client_len)) == -1) {
    fprintf(stderr, "ERROR: Can't accept client\n");
    exit(1);
  }
  
  printf("Got connection from client: %s on port %i\n", (char *)inet_ntoa(client.sin_addr), client.sin_port);
  
  
  while(1){
  	printf("Reading data from socket ...\n");
  	bzero(buf,BUFLEN);
  
  
  n = read(new_sd,buf,BUFLEN-1);
  if (n<0) {
    fprintf(stderr, "ERROR: Problem reading from socket");
    exit(1);
  }
  
  for( int k=0; k<(int)strlen(buf); k++){
  	inputData[k] = buf[k];
  }
  
  	if(executionCounter >= TOTALEXECUTIONS){
  		char totalExecutionsChar[3 + sizeof(char)];
  			
  		sprintf(totalExecutionsChar, "%d", executionCounter);
  		char byeResponse[BUFLEN - 1] = "Bye closing connection now we have executed ";
  		char operationString[12] = " operations";
  		strcat(byeResponse, totalExecutionsChar);
  		strcat(byeResponse, operationString);
  		strcpy(buf,byeResponse);
  		n = write(new_sd, buf, strlen(buf));
  			if (n<0) {
    				fprintf(stderr, "ERROR: Problem writing to socket");
    				exit(1);
  			}
  		printf("Closing the connection now due to execution count %d\n", executionCounter);
  		bzero(buf, BUFLEN);
  		break;
  	}
  	executionCounter++;
  
  if((inputData[0] == 'B' || inputData[0] == 'b') && (inputData[1] == 'Y' || inputData[1] == 'y') && (inputData[2] == 'E' || inputData[2] == 'e')){
  		char byeResponse[BUFLEN - 1] = "Bye closing connection now.";
  		bzero(buf, BUFLEN);
  		strcpy(buf,byeResponse);
  		n = write(new_sd, buf, strlen(buf));
  			if (n<0) {
    				fprintf(stderr, "ERROR: Problem writing to socket");
    				exit(1);
  			}
  		printf("Closing connection, received bye!\n");
  		bzero(buf, BUFLEN);
  		break;
  	}
  else{
  	int k =0;
  const int NUMBEROFOPERANDS = 8;
  int operand1 = 0, operand2 = 0, sign = 0, numberOfOperandsCounter = 0, operandToIncrease = 0, result = 0, haveAnOperand = 0, remainder = 0, numerator = 0, denominator = 1, 
  encounteredNonNumericChar = 0;
  int tempForOperandSize=0, operandSizeIndex = 0, sizePlacementDecrementor = 0;
  int sizeOfEachOperand[NUMBEROFOPERANDS];
  int inputOperands = 0, foundSecondOperand = 0, thirdOperand= 0,countingOperand = 0;
  
  
  printf(".. received [%s] (%d bytes) from client\n", buf,(int)strlen(buf));
  
  if(inputData[0] == 'a' || inputData[0] == 'A' || inputData[0] == 'D' || inputData[0] == 'd' || inputData[0] =='M' || inputData[0] == 'm' || inputData[0] == 'S' || inputData[0]=='s'){
  operand1 = 0, operand2 = 0, sign = 0, numberOfOperandsCounter = 0, operandToIncrease = 0, result = 0, haveAnOperand = 0, remainder = 0, numerator = 1, denominator = 1, 
  encounteredNonNumericChar = 0, tempForOperandSize=0, operandSizeIndex = 0, sizePlacementDecrementor = 1, inputOperands = 0;
  double exponent = 0;
  	for(int i =1; i < (int)strlen(buf); i++){
  		if(inputData[i] >= 48 && inputData[i] <=57 && haveAnOperand == 0){
  			tempForOperandSize = 1;
  			encounteredNonNumericChar = 0;
  			haveAnOperand = 1;
  			inputOperands = 1;
  			countingOperand = 1;
  		}
  		else if(inputData[i] >=48 && inputData[i] <= 57 && haveAnOperand != 0){
  			tempForOperandSize++;
  			if(encounteredNonNumericChar == 1){
  				foundSecondOperand = 1;
  			}
  			encounteredNonNumericChar = 0;
  			countingOperand = 1;
  		}
  		else if((inputData[i] < 48 || inputData[i] > 57) && encounteredNonNumericChar == 0 && haveAnOperand != 0){
  			sizeOfEachOperand[operandSizeIndex] = tempForOperandSize;
  			encounteredNonNumericChar = 1;
  			operandSizeIndex++;
  			if(foundSecondOperand != 0){
  				inputOperands++;
  			}
  			
  			tempForOperandSize = 0;
  			countingOperand = 0;
  		}
  		else{
  			continue;
  		}
  	}
  	if(foundSecondOperand != 0 && countingOperand == 1)
  		inputOperands++;
  	if(inputOperands == 0 || inputOperands == 1 && foundSecondOperand == 0){
  		char needMoreOperands[57] = "E:Please provide at least two operands, example A123/345";
  		bzero(buf, BUFLEN);
  		strcpy(buf, needMoreOperands);
  		n = write(new_sd, buf, strlen(buf));
  			if (n<0) {
    				fprintf(stderr, "ERROR: Problem writing to socket");
    				exit(1);
  			}
  		bzero(buf, BUFLEN);
  	}
  	else if(inputOperands > NUMBEROFOPERANDS){
  		char maxOperands[2 + sizeof(char)];
  		sprintf(maxOperands, "%d", NUMBEROFOPERANDS);
  		char tooManyOperands[52] = "E:I am sorry that's too many operands. Max is ";
  		strcat(tooManyOperands, maxOperands);
  		strcpy(buf, tooManyOperands);
  		n = write(new_sd, buf, strlen(buf));
  			if (n<0) {
    				fprintf(stderr, "ERROR: Problem writing to socket");
    				exit(1);
  			}
  		bzero(buf, BUFLEN);
  	}
  	else{
  	if(countingOperand == 1)
  		sizeOfEachOperand[operandSizeIndex] = tempForOperandSize;
  	
  	encounteredNonNumericChar = 0;
  	haveAnOperand = 0;
  	operandSizeIndex = 0;
  	foundSecondOperand = 0;
  	
  	switch(inputData[0]){
  		case'A':
  		case'a':
	  		result = 0;
  			for(int i =1; i < (int)strlen(buf); i++){
  				if(inputData[i] == 45 && sign == 0 && haveAnOperand == 0){
  					encounteredNonNumericChar = 1;
  					sign = -1;
  					numberOfOperandsCounter++;
  					operandToIncrease = numberOfOperandsCounter % 2;
  					haveAnOperand = 1;
  				}
  				else if(haveAnOperand == 0 && inputData[i] >= 48 && inputData[i]<=57){
  					numberOfOperandsCounter++;
  					operandToIncrease = numberOfOperandsCounter % 2;
  					haveAnOperand = 1;
  					operand1 = operand1 + ((inputData[i] - '0') * power(10, (sizeOfEachOperand[operandSizeIndex] - sizePlacementDecrementor)));
  					sizePlacementDecrementor++;
  				}
  				else if(inputData[i] >=48 && inputData[i] <= 57 && operandToIncrease == 1 && haveAnOperand != 0){
  					
  					encounteredNonNumericChar = 0;
  					countingOperand = 1;
  					if(sign == -1){
  						operand1 = operand1 - (inputData[i] -  '0') * power(10, (sizeOfEachOperand[operandSizeIndex] - sizePlacementDecrementor));
  						sizePlacementDecrementor++;
  					}
  					else{
  						operand1 = operand1 + ((inputData[i] - '0') * power(10, (sizeOfEachOperand[operandSizeIndex] - sizePlacementDecrementor)));
  						sizePlacementDecrementor++;
  					}
  				}
  				else if(inputData[i] >=48 && inputData[i] <= 57 && operandToIncrease == 0 && haveAnOperand != 0){
  					
  					encounteredNonNumericChar = 0;
  					countingOperand = 1;
  					if(sign == -1){
  						operand2 = operand2 - (inputData[i] -  '0') * power(10, (sizeOfEachOperand[operandSizeIndex] - sizePlacementDecrementor));
  						sizePlacementDecrementor++;
  					}
  					else{
  						operand2 = operand2 + (inputData[i] -  '0') * power(10, (sizeOfEachOperand[operandSizeIndex] - sizePlacementDecrementor));
  						sizePlacementDecrementor++;
  					}
  				}
  				else if(((inputData[i] < 48 || inputData[i] > 57) || inputData[i] == 45 || (inputData[i] >= 97 && inputData[i] <= 122)) && haveAnOperand != 0 && encounteredNonNumericChar == 0){
  					encounteredNonNumericChar = 1;
  					countingOperand = 0;
  					if(inputData[i] == 45){
  						sign = -1;
  					}
  					else{
  						sign = 0;
  					}
  					if(numberOfOperandsCounter >=2)
  					{
  						if(numberOfOperandsCounter == 2){
  							result = operand1 + operand2;
  						}
  						else if(numberOfOperandsCounter%2 == 1){
  							result = result + operand1;
  						}
  						else{
  							result = result + operand2;
	  					}
  						if(numberOfOperandsCounter%2 == 1){
  							operand2 = 0;
  						}
  						else{
  							operand1 = 0;
	  					}
  					}
  					sizePlacementDecrementor = 1;
  					operandSizeIndex++;
  					numberOfOperandsCounter++;
  					operandToIncrease = numberOfOperandsCounter % 2;
  				}
  				else if(inputData[i] == 45){
  					sign = -1;
  				}
  				else if(haveAnOperand == 0 && inputData[i] < 48 || inputData[i] > 57){
  					continue;
  				}
  				else if(encounteredNonNumericChar == 1 && inputData[i] < 48 || inputData[i] > 57){
  					continue;
  				}
  				else{
  					continue;
  				}
  			}
  			if(numberOfOperandsCounter == 2 && countingOperand == 1){
  				result = operand1 + operand2;
  			}
  			else if(numberOfOperandsCounter >= 3 && numberOfOperandsCounter % 2 == 1 && countingOperand == 1){
  				result = result + operand1;
  			}
  			else if (numberOfOperandsCounter > 3 && numberOfOperandsCounter %2 == 0 && countingOperand == 1){
  				result = result + operand2;
  			}
  			printf("The final result is %d\n\n", result);
  			char resultArray[MAXDIGITS + sizeof(char)];
  			strcat(buf, "= ");
  			sprintf(resultArray, "%d", result);
  			strcat(buf, resultArray);
  			strcpy(buf, buf);
  			
  			n = write(new_sd, buf, strlen(buf));
  			if (n<0) {
    				fprintf(stderr, "ERROR: Problem writing to socket");
    				exit(1);
  			}
  			break;
  		case'D':
  		case'd':
  			result = 1;
  			for(int i =1; i < (int)strlen(buf); i++){
  				if(inputData[i] == 45 && sign == 0 && haveAnOperand == 0){
  					encounteredNonNumericChar = 1;
  					sign = -1;
  					numberOfOperandsCounter++;
  					operandToIncrease = numberOfOperandsCounter % 2;
  					haveAnOperand = 1;
  				}
  				else if(haveAnOperand == 0 && inputData[i] >= 48 && inputData[i]<=57){
  					numberOfOperandsCounter++;
  					operandToIncrease = numberOfOperandsCounter % 2;
  					haveAnOperand = 1;
  					operand1 = operand1 + ((inputData[i] - '0') * power(10, (sizeOfEachOperand[operandSizeIndex] - sizePlacementDecrementor)));
  					sizePlacementDecrementor++;
  				}
  				else if(inputData[i] >=48 && inputData[i] <= 57 && operandToIncrease == 1 && haveAnOperand != 0){
  					encounteredNonNumericChar = 0;
  					countingOperand = 1;
  					if(sign == -1){
  						operand1 = operand1 - (inputData[i] -  '0') * power(10, (sizeOfEachOperand[operandSizeIndex] - sizePlacementDecrementor));
  						sizePlacementDecrementor++;
  					}
  					else{
  						operand1 = operand1 + ((inputData[i] - '0') * power(10, (sizeOfEachOperand[operandSizeIndex] - sizePlacementDecrementor)));
  						sizePlacementDecrementor++;
  					}
  				}
  				else if(inputData[i] >=48 && inputData[i] <= 57 && operandToIncrease == 0 && haveAnOperand != 0){
  					
  					encounteredNonNumericChar = 0;
  					countingOperand = 1;
  					if(sign == -1){
  						operand2 = operand2 - (inputData[i] -  '0') * power(10, (sizeOfEachOperand[operandSizeIndex] - sizePlacementDecrementor));
  						sizePlacementDecrementor++;
  					}
  					else{
  						operand2 = operand2 + (inputData[i] -  '0') * power(10, (sizeOfEachOperand[operandSizeIndex] - sizePlacementDecrementor));
  						sizePlacementDecrementor++;
  					}
  				}
  				else if(((inputData[i] < 48 || inputData[i] > 57) || inputData[i] == 45 || (inputData[i] >= 97 && inputData[i] <= 122)) && haveAnOperand != 0 && encounteredNonNumericChar == 0){
  					
  					encounteredNonNumericChar = 1;
  					countingOperand = 0;
  					if(inputData[i] == 45){
  						sign = -1;
  					}
  					else{
  						sign = 0;
  					}
  					if(numberOfOperandsCounter >=1)
  					{
  						if(numberOfOperandsCounter == 2){
  							numerator = operand1;
  							denominator = operand2;
  							result = numerator/denominator;
  							remainder = numerator % denominator;
  						}
  						else if(numberOfOperandsCounter%2 == 1){
  							numerator = numerator * operand1;
  						}
  						else{
  							denominator = denominator * operand2;
	  					}
  						if(numberOfOperandsCounter%2 == 1){
  							operand2 = 0;
  						}
  						else{
  							operand1 = 0;
	  					}
  					}
  					sizePlacementDecrementor = 1;
  					numberOfOperandsCounter++;
  					operandSizeIndex++;
  					operandToIncrease = numberOfOperandsCounter % 2;
  				}
  				else if(inputData[i] == 45){
  					sign = -1;
  				}
  				else if(haveAnOperand == 0 && inputData[i] < 48 || inputData[i] > 57){
  					continue;
  				}
  				else if(encounteredNonNumericChar == 1 && inputData[i] < 48 || inputData[i] > 57){
  					continue;
  				}
  				else{
  					printf("Denominator is %d.\n", operand2);
  					denominator = operand2;
  				}
  			}
  			if(numberOfOperandsCounter == 2 && countingOperand == 1){
  				denominator = 1 * operand2;
  			}
  			else if(numberOfOperandsCounter >= 3 && numberOfOperandsCounter % 2 == 1 && countingOperand == 1){
  				numerator = numerator * operand1;
  			}
  			else if (numberOfOperandsCounter > 3 && numberOfOperandsCounter %2 == 0 && countingOperand == 1){
  				denominator = denominator * operand2;
  			}
  			
  			
  			if(denominator == 0){
  				char doNotDivideByZero[42] = "E:I'm sorry but I cannot divide by zero!\n";
  				bzero(buf,BUFLEN);
  				strcpy(buf,doNotDivideByZero);
  				n = write(new_sd, buf, strlen(buf));
  				if (n<0) {
    					fprintf(stderr, "ERROR: Problem writing to socket");
    					exit(1);
  				}
  			}
  			else{
  			printf("Final numerator is %d and denominator %d\n", numerator, denominator);
  			result = numerator / denominator;
  			remainder = numerator % denominator;
  			printf("The final result is %d and remainder is %d\n\n", result, remainder);
  			
  			char resultArrayDivision[MAXDIGITS + sizeof(char)];
  			char remainderArray[MAXDIGITS + sizeof(char)];
  			
  			
  			sprintf(resultArrayDivision, "%d", result);
  			sprintf(remainderArray, "%d", remainder);
  			strcat(buf, "= ");
  			strcat(buf, resultArrayDivision);
  			strcat(buf, " remainder of ");
  			strcat(buf, remainderArray);
  			strcpy(buf,buf);
  			
  			n = write(new_sd, buf, strlen(buf));
  			if (n<0) {
    				fprintf(stderr, "ERROR: Problem writing to socket");
    				exit(1);
  			}
  			}
  			
  			
  			break;
  		case'M':
  		case'm':
  			result = 1;
  			for(int i =1; i < (int)strlen(buf); i++){
  				if(inputData[i] == 45 && sign == 0 && haveAnOperand == 0){
  					encounteredNonNumericChar = 1;
  					sign = -1;
  					numberOfOperandsCounter++;
  					operandToIncrease = numberOfOperandsCounter % 2;
  					haveAnOperand = 1;
  				}
  				else if(haveAnOperand == 0 && inputData[i] >= 48 && inputData[i]<=57){
  					numberOfOperandsCounter++;
  					countingOperand == 1;
  					operandToIncrease = numberOfOperandsCounter % 2;
  					haveAnOperand = 1;
  					operand1 = operand1 + ((inputData[i] - '0') * power(10, (sizeOfEachOperand[operandSizeIndex] - sizePlacementDecrementor)));
  					sizePlacementDecrementor++;
  				}
  				else if(inputData[i] >=48 && inputData[i] <= 57 && operandToIncrease == 1 && haveAnOperand != 0){
  					
  					encounteredNonNumericChar = 0;
  					countingOperand == 1;
  					if(sign == -1){
  						operand1 = operand1 - (inputData[i] -  '0') * power(10, (sizeOfEachOperand[operandSizeIndex] - sizePlacementDecrementor));
  						sizePlacementDecrementor++;
  					}
  					else{
  						operand1 = operand1 + ((inputData[i] - '0') * power(10, (sizeOfEachOperand[operandSizeIndex] - sizePlacementDecrementor)));
  						sizePlacementDecrementor++;
  					}
  				}
  				else if(inputData[i] >=48 && inputData[i] <= 57 && operandToIncrease == 0 && haveAnOperand != 0){
  					
  					encounteredNonNumericChar = 0;
  					countingOperand = 1;
  					if(sign == -1){
  						operand2 = operand2 - (inputData[i] -  '0') * power(10, (sizeOfEachOperand[operandSizeIndex] - sizePlacementDecrementor));
  						sizePlacementDecrementor++;
  					}
  					else{
  						operand2 = operand2 + (inputData[i] -  '0') * power(10, (sizeOfEachOperand[operandSizeIndex] - sizePlacementDecrementor));
  						sizePlacementDecrementor++;
  					}
  				}
  				else if(((inputData[i] < 48 || inputData[i] > 57) || inputData[i] == 45 || (inputData[i] >= 97 && inputData[i] <= 122)) && haveAnOperand != 0 && encounteredNonNumericChar == 0){
  					encounteredNonNumericChar = 1;
  					countingOperand = 0;
  					if(inputData[i] == 45){
  						sign = -1;
  					}
  					else{
  						sign = 0;
  					}
  					if(numberOfOperandsCounter >=2)
  					{
  						if(numberOfOperandsCounter == 2){
  							result = operand1 * operand2;
  						}
  						else if(numberOfOperandsCounter%2 == 1){
  							result = result * operand1;
  						}
  						else{
  							result = result * operand2;
	  					}
  						if(numberOfOperandsCounter%2 == 1){
  							operand2 = 0;
  						}
  						else{
  							operand1 = 0;
	  					}
  					}
  					sizePlacementDecrementor = 1;
  					operandSizeIndex++;
  					numberOfOperandsCounter++;
  					operandToIncrease = numberOfOperandsCounter % 2;
  				}
  				else if(inputData[i] == 45){
  					sign = -1;
  				}
  				else if(haveAnOperand == 0 && inputData[i] < 48 || inputData[i] > 57){
  					continue;
  				}
  				else if(encounteredNonNumericChar == 1 && inputData[i] < 48 || inputData[i] > 57){
  					continue;
  				}
  				else{
  					continue;
  				}
  			}
  			if(numberOfOperandsCounter == 2 && countingOperand == 1){
  				result = operand1 * operand2;
  			}
  			else if(numberOfOperandsCounter >= 3 && numberOfOperandsCounter % 2 == 1 && countingOperand == 1){
  				result = result * operand1;
  			}
  			else if (numberOfOperandsCounter > 3 && numberOfOperandsCounter %2 == 0 && countingOperand == 1){
  				result = result * operand2;
  			}
  			printf("The final result is %d\n\n", result);
  			char resultArrayMultiplication[MAXDIGITS + sizeof(char)];
  			
  			sprintf(resultArrayMultiplication, "%d", result);
  			strcat(buf, "= ");
  			strcat(buf, resultArrayMultiplication);
  			strcpy(buf,buf);
  			
  			n = write(new_sd, buf, strlen(buf));
  			if (n<0) {
    				fprintf(stderr, "ERROR: Problem writing to socket");
    				exit(1);
  			}
  			break;
  		case'S':
  		case's':
  			result = 0;
  			for(int i =1; i < (int)strlen(buf); i++){
  				if(inputData[i] == 45 && sign == 0 && haveAnOperand == 0){
  					encounteredNonNumericChar = 1;
  					sign = -1;
  					numberOfOperandsCounter++;
  					operandToIncrease = numberOfOperandsCounter % 2;
  					haveAnOperand = 1;
  				}
  				else if(haveAnOperand == 0 && inputData[i] >= 48 && inputData[i]<=57){
  					numberOfOperandsCounter++;
  					countingOperand = 1;
  					operandToIncrease = numberOfOperandsCounter % 2;
  					haveAnOperand = 1;
  					operand1 = operand1 + ((inputData[i] - '0') * power(10, (sizeOfEachOperand[operandSizeIndex] - sizePlacementDecrementor)));
  					sizePlacementDecrementor++;
  				}
  				else if(inputData[i] >=48 && inputData[i] <= 57 && operandToIncrease == 1 && haveAnOperand != 0){
  					
  					encounteredNonNumericChar = 0;
  					countingOperand = 1;
  					if(sign == -1){
  						operand1 = operand1 - (inputData[i] -  '0') * power(10, (sizeOfEachOperand[operandSizeIndex] - sizePlacementDecrementor));
  						sizePlacementDecrementor++;
  					}
  					else{
  						operand1 = operand1 + ((inputData[i] - '0') * power(10, (sizeOfEachOperand[operandSizeIndex] - sizePlacementDecrementor)));
  						sizePlacementDecrementor++;
  					}
  				}
  				else if(inputData[i] >=48 && inputData[i] <= 57 && operandToIncrease == 0 && haveAnOperand != 0){
  					
  					encounteredNonNumericChar = 0;
  					countingOperand = 1;
  					if(sign == -1){
  						operand2 = operand2 - (inputData[i] -  '0') * power(10, (sizeOfEachOperand[operandSizeIndex] - sizePlacementDecrementor));
  						sizePlacementDecrementor++;
  					}
  					else{
  						operand2 = operand2 + (inputData[i] -  '0') * power(10, (sizeOfEachOperand[operandSizeIndex] - sizePlacementDecrementor));
  						sizePlacementDecrementor++;
  					}
  				}
  				else if(((inputData[i] < 48 || inputData[i] > 57) || inputData[i] == 45 || (inputData[i] >= 97 && inputData[i] <= 122)) && haveAnOperand != 0 && encounteredNonNumericChar == 0){
  					encounteredNonNumericChar = 1;
  					countingOperand = 0;
  					if(inputData[i] == 45){
  						sign = -1;
  					}
  					else{
  						sign = 0;
  					}
  					if(numberOfOperandsCounter >=2)
  					{
  						if(numberOfOperandsCounter == 2){
  							result = 0 - operand1- operand2;
  						}
  						else if(numberOfOperandsCounter%2 == 1){
  							result = result - operand1;
  						}
  						else{
  							result = result - operand2;
	  					}
  						if(numberOfOperandsCounter%2 == 1){
  							operand2 = 0;
  						}
  						else{
  							operand1 = 0;
	  					}
  					}
  					sizePlacementDecrementor = 1;
  					operandSizeIndex++;
  					numberOfOperandsCounter++;
  					operandToIncrease = numberOfOperandsCounter % 2;
  				}
  				else if(inputData[i] == 45){
  					sign = -1;
  				}
  				else if(haveAnOperand == 0 && inputData[i] < 48 || inputData[i] > 57){
  					continue;
  				}
  				else if(encounteredNonNumericChar == 1 && inputData[i] < 48 || inputData[i] > 57){
  					continue;
  				}
  				else{
  					continue;
  				}
  			}
  			if(numberOfOperandsCounter == 2 && countingOperand == 1){
  				result = 0 - operand1 - operand2;
  			}
  			else if(numberOfOperandsCounter >= 3 && numberOfOperandsCounter % 2 == 1 && countingOperand == 1){
  				result = result - operand1;
  			}
  			else if (numberOfOperandsCounter > 3 && numberOfOperandsCounter %2 == 0 && countingOperand == 1){
  				result = result - operand2;
  			}
  			printf("The final result is %d\n\n", result);
  			char resultArraySubtraction[MAXDIGITS + sizeof(char)];
  			
  			sprintf(resultArraySubtraction, "%d", result);
  			strcat(buf, "= ");
  			strcat(buf, resultArraySubtraction);
  			strcpy(buf,buf);
  			
  			n = write(new_sd, buf, strlen(buf));
  			if (n<0) {
    				fprintf(stderr, "ERROR: Problem writing to socket");
    				exit(1);
  			}
  			break;
  		default:
  			printf("This should never print out due to only valid options allowed!\n");
  	}
  	}
  }
  else{
  	bzero(buf,BUFLEN);
  	char invalidInputResponse[BUFLEN -1] = "That's an invalid input, you can use: a,s,d,m or A,S,D,M followed by the operands or bye/Bye to close connection\n";
  	strcpy(buf,invalidInputResponse);
  	n = write(new_sd, buf, strlen(buf));
  	if (n<0) {
    		fprintf(stderr, "ERROR: Problem writing to socket");
    		exit(1);
  	}
  }
  }
  
  }
  
  
  
  
  
  /* Clean up and exit */
  
  close(new_sd);
  close(sd);
  return(0);
}

int power(int base, int exponent){
	int result = 1;
	for(int i =0; i < exponent; i++){
		result*= base;
	}
	return result;
}

