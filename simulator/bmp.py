#!/usr/bin/python3

# BMP - BACON Message Protocol

'''
Here's how this works:
    Uppercase letter: request
    Lowercase letter: response to request

All message types are in MSG_TYPES. Simulator doesn't care about
PAYLOAD_ALT. Simulator response to WAT_REQUEST with a WAT_REPLY + "S" for "sim".
'''

MSG_TYPES = {
    'SIM_ALT':      's',        # simulated altitude.
    'PAYLOAD_ALT':  'a',        # current payload altitude
    'ALT_REQUEST':  'R',        # altitude request. includes compensation amount.
    'WAT_REQUEST':  'W',        # 'Who Art Thou' request. think ARP, but instead of asking for one addr, it asks for all.
    'WAT_REPLY':    'w',        # 'Who Art Thou' reply. send "RS" if sim, "RP" if payload.
}

def sim_alt_str(alt):
    ''' alt should be a signed 32-bit integer. '''
    print(MSG_TYPES['SIM_ALT'] + alt)

def parse(msg):
    ''' msg should be a RX frame. msg[15] is the first data byte. '''
    if msg[3] != 0x90:
        raise Exception("parse_request(): Non-RX frame passed!")

    if msg[15] == MSG_TYPES['ALT_REQUEST']:
        # payload is requesting altitude. msg[1:-1] is the time it
        # has opened its valve since the last request, in ms.
        ret = (MSG_TYPES['ALT_REQUEST'], msg[15:-1])

    elif msg[15] == MSG_TYPES['WAT_REQUEST']:
        # ret[1] is the string to send back as a WAT_REPLY.
        # we are a simulator, hence "S"
        ret = (MSG_TYPES['WAT_REQUEST'], MSG_TYPES['WAT_REPLY'] + "S")

    elif msg[15] == MSG_TYPES['WAT_REPLY']:
        # ret[1] is the device type (probably P for payload) and ret[2] is its address.
        ret = (MSG_TYPES['WAT_REPLY'], msg[16], ret[4:11])

    else:
        # Not a bmp message. At least not one we, the simulator, are interested in.
        ret = (None,)

    return ret
