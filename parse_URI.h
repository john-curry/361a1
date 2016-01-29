#ifndef PARS
#define PARS
 /* Function: parse_URI

  * Args: an http uri. Quits if there is no http header specified
          at the beginning of the uri.

  * Return: hostname, port and identifier. Quits if there is no hostname. Port defaults to 80 and identifier defaults to index.html

  * Notes: 
*/


void parse_URI(char *uri, char *hostname, int *port, char *identifier)
{
  char * d1 = ":";
  char * d2 = "/";
    
  // resist mangling the input uri
  char * uri_cpy = (char *)malloc(sizeof(char) * strlen(uri));
  strcpy(uri_cpy, uri);

  // make sure we are parsing an http uri
  char * protocol = strtok(uri_cpy, d1); // read up to the first colon "/"

  if (strcmp(protocol, "http") != 0) {
    printf("Expected http protocol. Got %s\n", protocol);
    goto error_exit;
  }

  char * host = strtok(NULL, ":/"); // read up to the first slash "/ or colon"
  
  while (host[0] == '/') host++; // get rid of the leading slash slash "//"

  char * next_str = strtok(NULL, d2);
  char * id;
  if (next_str == NULL) {
    *port = 80;
    id = "index.html";
  } else {
    *port = atoi(next_str);
    if (*port <= 0) {
      id = next_str;
      *port = 80;
    } else {
      id = strtok(NULL, " \n\r:");
    }
  }
  
  if (id != NULL) {
    strcpy(identifier, id);
  } else {
    identifier = "index.html"; 
  }
  if (host != NULL) {
    strcpy(hostname, host);
    goto clean_exit;
  } else {
    puts("ERROR: Expected a hostname got NULL\n");
    goto error_exit;
  }

  clean_exit:
  free(uri_cpy);
  return;

  error_exit:
  free(uri_cpy);
  exit(1);
}
#endif
