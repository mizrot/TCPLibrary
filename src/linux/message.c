#include"message.h"
#include<string.h>

int pack_msg(const char * const string, socket_t dest, message_t *msg){
   if (!msg || !string || dest == INVALID_SOCKET){
	return -1;
   }
   size_t len = strlen(string);

   msg->socket = dest;
   msg->len = strlen(string);

   memcpy(msg->string, string, len);

   return 0;
}

int unpack_msg(message_t *msg, char *buff, size_t buff_size){
   if (!buff || !msg || msg->socket == INVALID_SOCKET){
	return -1;
   }
   if (buff_size < msg->len+1){
	return -1;
   }
   memcpy(buff, msg->string, msg->len+1);
   buff[msg->len] = '\0';
   return 0;
}
