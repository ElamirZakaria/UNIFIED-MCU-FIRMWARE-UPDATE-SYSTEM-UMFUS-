import socketio
from hex_json import hex_serialize, hex_deserialize

from cryptography.fernet import Fernet

# Generate a random encryption key
key = b'HPfamgIJB2gGsC1xPuK2y8DFwBsXMwBtdigSjzUvRgw='

#print(key)

# Create a Fernet cipher object with the key
cipher = Fernet(key)

def encrypt_string(plain_text):
    # Encode the plain text string to bytes
    plain_text_bytes = plain_text.encode('utf-8')

    # Encrypt the bytes using the cipher
    encrypted_bytes = cipher.encrypt(plain_text_bytes)

    # Convert the encrypted bytes to a string
    encrypted_string = encrypted_bytes.decode('utf-8')

    return encrypted_string

def decrypt_string(encrypted_string):
    # Encode the encrypted string to bytes
    encrypted_bytes = encrypted_string.encode('utf-8')

    # Decrypt the bytes using the cipher
    decrypted_bytes = cipher.decrypt(encrypted_bytes)

    # Convert the decrypted bytes to a string
    decrypted_string = decrypted_bytes.decode('utf-8')

    return decrypted_string


# standard Python
sio = socketio.Client()



@sio.on('connect')
def connect():
    print('Client Connected')


@sio.on('disconnect')
def disconnect():
    print('Client Disconnected')


@sio.on('message')
def message(data):
    print(f'I received a message: {data}')

@sio.on('W_connectCar')
def handle_sokcet(W_connectCar):
    if(W_connectCar == "Connection Accepted"):
        print("Car Connected")
    else:
        print(W_connectCar)

# Connecting to ECU
@sio.on("W_connectECU")
def handle_socket(W_connectECU):
    if(W_connectECU == "Connection Established With ECU"):
        print("ECU Connected")
    else:
        print(W_connectECU)

@sio.on('W_recieveHex')
def handle_socket(W_recieveHex):
    if(W_recieveHex == "Received Successfully"):
        print("Hex Received Successfully")



# sOpen Hex File and Split it into lines
hex_list = []
hex_file = open('Serialized.txt', 'r')
hex_serialized = hex_file.read()
hex_list = hex_serialized.split(',')

# hex_s = hex_serialize(hex_list)

encrypted_string = encrypt_string(hex_serialized)

print(encrypted_string)

decrypted_string = decrypt_string(encrypted_string)

print(decrypted_string)

hex_splitted = decrypted_string.split(',')



while True:
    event_name = input("Enter Event Name: ")
    

    if(event_name == " " or event_name == "" or event_name == '\n' or event_name == "\n"):
        print("Invalid Input")
        continue

    if(event_name == 'connect'):
        sio.connect('http://192.168.1.20:55555')
        sio.emit('join', 'website')
    elif(event_name == 'disconnect'):
        sio.disconnect()
    elif(event_name == "send hex"):
        sio.emit("W_recieveHex", encrypted_string)
    else:
        event_data = input("Enter Event Data: ")
        if(event_data == " " or event_data == "" or event_data == '\n' or event_data == "\n"):
            print("Invalid Input")
            continue
        sio.emit(event_name, event_data)
