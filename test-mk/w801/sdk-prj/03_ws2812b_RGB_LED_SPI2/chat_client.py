#!/usr/bin/python
# chat_client.py

import sys, socket, select
 
def chat_client():
    #host = '192.168.1.33'
    host = '192.168.1.116'
    port = 5555

    #if(len(sys.argv) < 3) :
        #print( 'Usage : python chat_client.py hostname port' )
        #sys.exit()

    if(len(sys.argv) == 3) :
    	host = sys.argv[1]
    	port = int(sys.argv[2])
     
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(2)
     
    # connect to remote host
    try :
        s.connect((host, port))
    except :
        print( 'Unable to connect' )
        sys.exit()
     
    print( 'Connected to remote host. You can start sending messages' )
    sys.stdout.write('[Me] '); sys.stdout.flush()
     
    while 1:
        socket_list = [sys.stdin, s]
         
        # Get the list sockets which are readable
        read_sockets, write_sockets, error_sockets = select.select(socket_list , [], [])
         
        for sock in read_sockets:            
            if sock == s:
                # incoming message from remote server, s
                data = sock.recv(4096)
                if not data :
                    print( '\nDisconnected from chat server' )
                    sys.exit()
                else :
                    #print data
                    sys.stdout.write(data.decode())
                    sys.stdout.write('[Me] '); sys.stdout.flush()     
            
            else :
                # user entered a message
                msg = sys.stdin.readline()
                s.send(msg.encode())
                sys.stdout.write('[Me] '); sys.stdout.flush() 

if __name__ == "__main__":

    sys.exit(chat_client())

