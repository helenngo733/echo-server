# echo-server

Using TCP, a server that echoes all messages it receives, line by line.

If you send "abcd"(newline), you should get back "abcd"(newline). Then type another line, the same thing. As long as the connection stays alive. 

Provide a "-p" argument that specifies the port it listens to.

It only accept one connection at a time, but accepts new connections after one is terminated.

Test this with telnet; probably need to install it.

Highly recommended: test it with your web browser, too.
