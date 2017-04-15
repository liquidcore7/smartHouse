# smartHouse
### Simple server to control RaspberryPi-driven house

# API
##### You can control your house with GET and POST requests to the server.
Avaliable requests:
  * /auth (**POST**) : log in. Default password is *raspHouse*, you can change it in server/smartHouseApi.hpp:10
  * /try (**GET**) : test connection
  * /getstate (**GET**) : get states of all pins
  * /setlight1 (**POST**) : set the lamp on GPIO2 to "ON" or "OFF"
  * /setlight2 (**POST**) : set the lamp on GPIO3 to "ON" or "OFF"
  * /setdoor (**POST**) : set the door magnet on GPIO4 to "OPEN" or "CLOSE"
  * /unauth (**POST**) : log out, do this on session end. Leave data empty
  

