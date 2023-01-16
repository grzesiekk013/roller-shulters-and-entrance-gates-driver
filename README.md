# roller-shulters-and-entrance-gates-driver
Client &amp; Server software/firmware used for roller shutters and entrance gates

![androidAppIco128](https://user-images.githubusercontent.com/90092477/211022856-fb7a1996-3683-4325-965b-60db5505ddb4.png)

About:

I wrote a client side and a server side to control with current switches. The main idea is to automate the closing of anti-burglary blinds when dusk comes or a certain hour strikes. When dawn comes, the blinds are supposed to go up so that sunlight enters through the windows of our houses. The server side was written in C ++, due to the performance and unstable firmware for the Raspberry Pi Pico W, which, programmed with the Micropython language, could crash. I decided to write the programs on the client side in c#, due to the familiarity with this language and the simplicity of creating multiplatform applications. In addition, the programs have been equipped with the ability to control the blinds and entrances to the house using the application for mobile devices. Thanks to this, we will be able to control anti-burglary blinds from every corner of the house. The principle of operation is simple. The IoT device listens to see if the client side has sent an HTTP request with a given path, e.g. all/up. If an Iot device intercepts such a request, it opens and closes switches simulating pressing a button, isn't it simple?

Setup:
  1) Server side
  
     Enter your SSID and password into config
    
     Generate and update Secret Key password (you can use sth. like 
     www.avast.com/random-password-generator but don't use special characters)
    
     Edit code
    
     Flash IoT device (like ESP8266 or Raspberry PICO W)
    
  2) Client side
  
     Download & install (if needed) client side software for Windows/Android
    
     For Windows: Edit <>.cs file. Install. Open ClientConfig.exe ( edit server ip & port if needed )
    
     For Android: Edit <>.cs file. Install. Open app, then go to Settings, set connection type and edit server ip & port if needed
    
Wiring example:


