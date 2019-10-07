*** Settings ***
Documentation    Suite description
Library  CommunicationAPI.py
Library  Message.py
Suite Setup  Prepare dev
Suite Teardown  Disconnect dev


*** Variables ***
${path}            /dev/ttyUSB0
${baud_rate}       ${9600}
${led_pin}         ${18}
${message_length}  ${11}


*** Test Cases ***
led on
    [Documentation]  Send led turning on command, then check if it happened.
    turn on led
    send command
    sleep  4s

    confirm 1 ack messages has been sent for led power

    ${pin_state} =  get input gpio state  ${led_pin}
    should be true  ${pin_state}

led off
    [Documentation]  Send led turning off command, then check if it happened.
    turn off led
    send command
    sleep  4s

    confirm 1 ack messages has been sent for led power

    ${pin_state} =  get input gpio state  ${led_pin}
    should not be true  ${pin_state}

Led switches in one message send
    [Documentation]  Increasingly send more messages in one command until it fails.
    FOR    ${i}    IN RANGE    1    6
        log to console  Iteration ${i}
        reset counter
        count when pin state changes  ${led_pin}
        Make ${i} led switches
        send command
        sleep  13s
        run keyword and ignore error  confirm ${i} ack messages has been sent for led power
        ${led_changes} =  get counter
        run keyword and ignore error  should be equal as integers  ${led_changes}  ${i}   Less than ${i} messages are possible to execute at once.
        reset pin notifiers  ${led_pin}
    END

Device shouldn't restart after message flooding
    [Documentation]  Spam messages to the device, check if it's working stable.
    spam led on off  ${led_pin}  0.1  10
    ${data} =  read data
    ${data} =  convert to string  ${data}
    log to console  ${data}
    Should not contain  ${data}  start !

Send 5 messages and expect 'busy' response
    [Documentation]  After sending 5 commands, a device should inform us that it's busy
    make 5 led switches
    send command
    sleep  10s
    confirm receiving busy message

*** Keywords ***
Prepare dev
    [Documentation]  Configures port parameters and sets led pin as input pin
    configure port  ${path}  ${baud_rate}
    set gpio state  ${led_pin}
    read data

Disconnect dev
    [Documentation]  Disconnects with device
    close connection

Make ${i} led switches
    FOR    ${index}  IN RANGE   0  ${i}
        switch led state  ${led_pin}
    END

Confirm ${i} ACK messages has been sent for led power
    FOR  ${index}  IN RANGE  0  ${i}
        ${data} =  read data  ${message_length}
        ${message} =  convert to message  ${data}
        should be equal  ${message.command}  ${0}
        should be equal  ${message.data_type}  ${7}
    END

Confirm receiving busy message
    ${data} =  read data  ${message_length}
    ${message} =  convert to message  ${data}
    should be equal  ${message.command}  ${0}
    should be equal  ${message.data}  ${1}
    should be equal  ${message.data_type}  ${7}

