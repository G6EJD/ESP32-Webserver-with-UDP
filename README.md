# ESP32-Webserver-with-UDP
An ESP32 webserver combined with UDP to control client devices

1. Programme an ESP32 with the Server code to act as the central hub controller. Choose a UDP broadcast IP that matches your network. For most purposes the port number 3333 will not conflict with any other port.

2. Programme ESP32's to be the Client devices, ensure the address chosen matches those used in the server so currently in the range C01, C02, C03 and so-on, Make sure each Client has its own address e.g. C01

3. Extending to get command numbers can be donw like this:

a. Modify commands in the server such as C01H to be for example C01077 e.g. for 77%

b. Modify the Client to receive the command and extract the sting after the Client address e.g. C01077 would be extracted like this:

    i. Client address = receivedcommand.subString(0,2); // Now contains 'C01'
    
    ii. Command = receivedcommand.substring(3); // Now contains '077'
    
    iii. int command_value = Command.toInt();
    
    iv.  servo.write(command_value);
