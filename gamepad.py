import socket                      # for network communiccation
import vgamepad as vg              # for emulating XBOX 360 gamepad


def debg(str):                      # for debugging/error messages
    if debgEnable:
        print(str)


def addGamepad(name):               # add the gamepad to the gamepad dictionary and if already there then return
    if name in gamepad.keys():
        return
    gamepad[name] = [vg.VX360Gamepad(), 0]  # add an entry to the dictionary with the value of the list here


buttonsToUse = [vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_UP, vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_DOWN,
                vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_LEFT, vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_RIGHT]      # All the buttons of XBOX 360 Controller used 


def updateGamepad(name, buttonState, joyVal):       # Updates the button presses of the joystick
    if name not in gamepad.keys():
        return
    
    gamepad[name][0].reset()                        # Clear any previously set buttons
    for i in range(4):                              # 4 buttons UDLF
        if buttonState[i] == '1':                   # if is pressed
            gamepad[name][0].press_button(button=buttonsToUse[i])   # simulate the pressing of the button from the list of buttons

    joy = joyVal.split(",")                         # Extract X and Y values to a list 'joy'
    if len(joy) < 2:                                # If Incomplete data then exit the function
        gamepad[name][0].update()
        return
    
    joyX = float(joy[0])
    joyY = float(joy[1])
    
    if joyX > 1:                                    # Normalise to int values
        joyX = 1
    elif joyX < -1:                                 # Normalise to int values
        joyX = -1

    if joyY > 1:
        joyY = 1                                    # Normalise to int values
    elif joyY < -1:
        joyY = -1                                   # Normalise to int values

    gamepad[name][0].left_joystick_float(x_value_float=joyX, y_value_float=joyY)        # Set values of JoyStick as -1/0/1 
    gamepad[name][0].update()                       # Update 


gamepad = {}                                        # Dictionary
debgEnable = True                                   # Debug ON/OFF

localIP = "192.168.217.81"                          # Common IP OF HOST AND DEVICE [get by 'ipconfig']
localPort = 5555                                    # Common Port of Reciever listed on both ESP and System
bufferSize = 1024                                   # Data buffer [Data read in one operation]

msgFromServer = "Hello UDP Client"                  # Sent from SYS to ESP
bytesToSend = str.encode(msgFromServer)             # String to bytes


# Create a UDP socket for communication using IPv4
# - family=socket.AF_INET: Use IPv4 addresses
# - type=socket.SOCK_DGRAM: Use UDP for sending and receiving messages
# - proto=socket.IPPROTO_UDP: Specify UDP as the protocol
UDPServerSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM, proto=socket.IPPROTO_UDP)


UDPServerSocket.bind((localIP, localPort))          # Bind to address and ip
print("UDP server up and listening")


# Listen for incoming datagrams
while True:
    bytesAddressPair = UDPServerSocket.recvfrom(bufferSize)     # Recieve Data as a tuple (recieved data, sender address)
    message = bytesAddressPair[0]
    address = bytesAddressPair[1]

    # print(f"IP::{address} :: msg :: {message}")               # print the ESP's ip

    try:
        message = message.decode('utf-8')                       # decode the message [convert bytes to string]
    except():
        continue                                                # for catching errors

    inp_split = message.split("|")                              # message == name|UPDOWNLEFTRIGHT|JX,JY
    
    if not (len(inp_split) == 3):                               # check for error , if incomplete data arrived
        debg("not 3")                                           # debug message
        continue

    addGamepad(inp_split[0])                                    # gamepad name from ESP 'MyName'    
    updateGamepad(inp_split[0], inp_split[1], inp_split[2])     # update gamepad function called with parametes -> name,buttonValues,joyValues

