/************************************************* Crypto Coin Tracker Project With NodeMCU - 16x2 LCD ********************************************************/
// Dont forget to download libraries before compile the program!!! 
// Firstly enter  https://www.thingspeak.com and create your API Links AND KEYS to get data of your coins
// After that enter the name and password of your WIFI
// Next step is just put the api links to the their places in the code. If you wanna follow more coins, you can upgrade and optimize the code!
// We add switch to D7 digital pin to switch between coins easily. 
// When you add your switch, you can add a pull-down resistor to gain better result!
// This project still being developed by the author!!!!
// For more Information, you can get contact with me
// https://worldclasslab.wordpress.com/
/***********************************   For More Information About ----->   ThingSpeak ( https://www.thingspeak.com )  ******************************************/
    
    int countRST = 0;                                                             // Counting variable for reset function to trigger 
    int ResetPin = 16;                                                            // Have to unpluged the cable to RST pin during uploading code !!! 

    #define  Switch    D7
    #include <Wire.h> 
    #include <LiquidCrystal_I2C.h>
    #include <ESP8266WiFi.h>                                                      // library for wifi configuration codes.
                                                                                  
  
    LiquidCrystal_I2C lcd(0x27,20,4);                                             // set the LCD address to 0x27 for a 16 chars and 2 line display
                                                                                      
    const char* ssid = "router name";                                             // key in your wifi-router name
    const char* password =  "password";                                           // key in your wifi-router password
    WiFiClient client;                                                            


    #include <ESP8266HTTPClient.h>                                           
    int trimPosition;                                                             // library for HTTP communication codes. Will automatically pre-installed from NoveMCU board manager
    
 
void setup() 
{
    Serial.begin(9600);                                                           // initiate Serial monitor with baud rate 9600

    lcd.init();                                                                   // initialize the lcd 
    lcd.backlight();
    lcd.clear();
    
    /* 0- Reset Board */
    pinMode(Switch, INPUT);      
    pinMode(ResetPin, OUTPUT);                                                    // to define pin as output function 
    digitalWrite(ResetPin, HIGH);                                                 // set pin as high output 


    /* 1- ESP8266 WiFi connection */
    
    WiFi.disconnect();                                                            // disconnects any previous Wifi connection
    delay(1000);                                                                  
    WiFi.begin(ssid, password);                                                   // initiate Wifi connection with your router 
    while (WiFi.status() != WL_CONNECTED)                                         // if connection not completed, keep run the codes 
      {   
        delay(1000); Serial.println("Connecting to WiFi.."); 
            
        /* 0- Reset Board */ 
        
        countRST = countRST + 1;                                                  // add counting time, if waited too long (by default 1 minute), reset the board
        if (countRST == 60)
        { digitalWrite(ResetPin, LOW); countRST = 0; }  
      }
    Serial.println("Connected to the WiFi network");                                            
    Serial.println(WiFi.localIP());                                               // print IP address of Node Mcu on Serial Monitor 
  
}

 
void loop()                                                                      
{

  
    while(digitalRead(Switch) == 0) {
      
    const char* host = "api.thingspeak.com";                                      // The main server HTTP URL where you read the website value
    const int httpPortRead = 80;                                                  
    const char* url = "ADD YOUR API LINK";                                        // Get the API link from ThingSpeak (ThingHTTP App), have to exclude the main HTTP URL
    //const char* url2 = ".......";                                              
                                                                                                                                                                 
    HTTPClient http;                                                          
    //HTTPClient http2;                                                           
                                                                                  
                                                                                  
    
    
    String apiKey = "YOUR API KEY";                                           // Enter your Write API key from ThingSpeak channel. Each channel will have different Write Api Key
    const char* server = "api.thingspeak.com";                                    // In this project we are using Web Server broker: thingspeak.com
    const int httpPortWrite = 80;                                                 

  
   countRST = 0;                                                                  // use to trigger reset board if connection takes too long
   
          
    while (WiFi.status() != WL_CONNECTED)                                         // if wifi connection not completed or disconnection 
      { 
        if(countRST ==0)                                                          // for the first time if disconnection detected
        {
        WiFi.disconnect();                                                        // disconnect any previous connection
        delay(1000);                                                              
        }
        WiFi.begin(ssid, password);                                               // initiate Wifi connection with your router 
        Serial.println("Reconnecting to WiFi..");       
        delay(20000);                                                             // let it has 20 second time for processing 
            
        /* 0- Reset Board */
            
        countRST = countRST + 1;                                                  
        if (countRST == 15)                                                        
        { digitalWrite(ResetPin, LOW); countRST = 0; }                           
      }

    
    while(!client.connect(host,httpPortRead))                                         // if failed to connect to the web server
      {
        client.connect(host, httpPortRead);                                           // tries to connect to server
        countRST = countRST + 1;                                                      // record how many tries
        delay(5000);                                                                  // wait for 10 seconds
        if (countRST == 30)                                                           // if waited too long (by default 5 minutes has reached)
          { 
            WiFi.disconnect();                                                        // disconnect wifi
            delay(2000);                                                          
            WiFi.begin(ssid, password);                                               // start connect wifi again
            countRST = 0;                                                             // reset the counting time
          }
      }
    
    if( http.begin(host,httpPortRead,url))                                            // If the connection is ok for first value, for more values this set of code need to duplicate
      {
        int httpCode = http.GET();                                                    // Check HTTP return code
        if (httpCode > 0)                                                             // If return code is good
        {
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) 
            {
               
              
              String payload = http.getString();                                      // get the return value in string
                                                                                    
                                                                                           
        lcd.setCursor(0,0);
        lcd.print("Graph - ");
        lcd.setCursor(9,0);
        payload.remove(0,3);
        lcd.print(payload);
        Serial.println(payload);

            }
        }
        else 
        {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }


        
    delay(1000);                                                                     // update every 10 minutes      
}

    host = "api.thingspeak.com";                                      // The main server HTTP URL where you read the website value.
    url = "ADD YOUR API LINK";                                        // Get the API link from ThingSpeak (ThingHTTP App), have to exclude the main HTTP URL
    //const char* url2 = ".......";                                               
                                                                                                                                                              
   
       
    apiKey = "YOUR API KEY";                                           // Enter your Write API key from ThingSpeak channel. Each channel will have different Write Api Key
    server = "api.thingspeak.com";                                    

  
   countRST = 0;                                                                 
    
    while(!client.connect(host,httpPortRead))                                     
      {
        client.connect(host, httpPortRead);                                       
        countRST = countRST + 1;                                                 
        delay(7000);                                                             
        if (countRST == 30)                                                     
          { 
            WiFi.disconnect();                                                    
            delay(2000);                                                         
            WiFi.begin(ssid, password);                                          
            countRST = 0;                                                        
          }
      }
    
    if( http.begin(host,httpPortRead,url))                                               // If the connection is ok for first value, for more values this set of code need to duplicate
      {
        int httpCode = http.GET();                                                       // Check HTTP return code
        if (httpCode > 0)                                                                // If return code is good
        {
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) 
            {
              
              // To find first value (total cases) from return value : 75,779 2,13016,917  
              
              String payload2 = http.getString();                                        
                                                                                        
                                                                                              
        lcd.setCursor(0,1);
        lcd.print("EOS - ");
        lcd.setCursor(8,1);
        payload2.remove(0,3);
        lcd.print(payload2);

            }
        }
        else 
        {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }


        
    delay(1000);                                                               
}

    if(digitalRead(Switch) == 1) {
      lcd.clear();
    }
                 
    }
    
    while(digitalRead(Switch) == 1) {
      
    const char* host = "api.thingspeak.com";                                      
    const int httpPortRead = 80;                                                  
    const char* url = "ADD YOUR API LINK";    // Get the API link from ThingSpeak (ThingHTTP App)
    //const char* url2 = ".......";                                               
                                                                                                                                                               
    HTTPClient http;                                                              
    //HTTPClient http2;                                                          
                                                                                  
                                                               
    
    String apiKey = "ADD YOUR API KEY";                                           // Enter your Write API key from ThingSpeak channel. 
    const char* server = "api.thingspeak.com";                                    
    const int httpPortWrite = 80;                                                 

  
   countRST = 0;                                                                  
   
          
    while (WiFi.status() != WL_CONNECTED)                                         // if wifi connection not completed or disconnection
      { 
        if(countRST ==0)                                                         
        {
        WiFi.disconnect();                                                      
        delay(1000);                                                              
        }
        WiFi.begin(ssid, password);                                              
        Serial.println("Reconnecting to WiFi..");       
        delay(20000);                                                           
            
      
            
        countRST = countRST + 1;                                               
        if (countRST == 15)                                                      
        { digitalWrite(ResetPin, LOW); countRST = 0; }                            
      }


    
    
    while(!client.connect(host,httpPortRead))                                       // if failed to connect to the web server
      {
        client.connect(host, httpPortRead);                                         // tries to connect to server
        countRST = countRST + 1;                                                 
        delay(5000);                                                             
        if (countRST == 30)                                                      
          { 
            WiFi.disconnect();                                                   
            delay(2000);                                                         
            WiFi.begin(ssid, password);                                           
            countRST = 0;                                                       
          }
      }
    
    if( http.begin(host,httpPortRead,url))                                        
      {
        int httpCode = http.GET();                                                     
        if (httpCode > 0)                                                              
        {
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) 
            {
              
       
              
              String payload3 = http.getString();                                       
                                                                                    
                                                                                      
        lcd.setCursor(0,0);
        lcd.print("Reef    - ");
        lcd.setCursor(10,0);
        payload3.remove(0,3);
        lcd.print(payload3);
        Serial.println(payload3);

            }
        }
        else 
        {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }


        
    delay(1000);                                                                    // update every 10 minutes      
}

    host = "api.thingspeak.com";                                      
    url = "/apps/thinghttp/send_request?api_key=L6P4WISZ0BE0Q589";    
    //const char* url2 = ".......";                                              
                                                                                                                                                           
   
    
    
    apiKey = "YOUR API KEY";                                                   // Enter your Write API key from ThingSpeak channel
    server = "api.thingspeak.com";                                    

  
   countRST = 0;                                                                  

    
    while(!client.connect(host,httpPortRead))                                      // if failed to connect to the web server
      {
        client.connect(host, httpPortRead);                                       
        countRST = countRST + 1;                                                  
        delay(7000);                                                             
        if (countRST == 30)                                                        // if waited too long (by default 5 minutes has reached)
          { 
            WiFi.disconnect();                                                    
            delay(2000);                                                          
            WiFi.begin(ssid, password);                                           
            countRST = 0;                                                         
          }
      }
    
    if( http.begin(host,httpPortRead,url))                                              
      {
        int httpCode = http.GET();                                                      
        if (httpCode > 0)                                                               
        {
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) 
            {
              
              
              String payload4 = http.getString();                                       
                                                                                        
                                                                                            
        lcd.setCursor(0,1);
        lcd.print("JulSwap - ");
        lcd.setCursor(10,1);
        payload4.remove(0,3);
        lcd.print(payload4);

            }
        }
        else 
        {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }


        
    delay(1000);                                                                      // update every 10 minutes      
      }

      if(digitalRead(Switch) == 0) {
      lcd.clear();
    }
                 
    }
      
            
    
    delay(10);
}
