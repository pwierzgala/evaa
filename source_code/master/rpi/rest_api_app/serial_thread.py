from queue import Queue, Empty
from serial import Serial
from threading import Thread, Event
from time import sleep
from message_handler import *
from struct import *
from message import Message


class Sender:
    def __init__(self, device):
        self.send_queue = Queue()
        self.send_message_flag = Event()
        self.device = device

    def write_to_device(self):
        message = self.send_queue.get_nowait()
        self.device.write(message)
        logging.info("Message has been sent!")
        logging.info(message)
        self.send_message_flag.clear()


class Receiver:
    def __init__(self, device):
        self.receive_queue = Queue()
        self.receive_message_flag = Event()
        self.messages_buffer = list()
        self.device = device
        self.handler = MessageHandler()

    def receive_message(self):
        try:
            command = self.receive_queue.get()
            data_type = self.receive_queue.get()
            message = self.receive_queue.get()

            logging.debug("Received message. Command: %s. Data_type: %s.",
                          command, data_type)
            self.handler.process(command, data_type, message)
        except Empty:
            logging.warning('Receive queue is empty!')
        self.receive_message_flag.clear()

    def handle_response(self):
        data = self.device.read(Message.length)
        logging.info("Data has been read: %s", data)
        unpacked_data = unpack('<11B', data)

        chunk_count, chunk_index, command, data_type = unpacked_data[2:6]

        if chunk_index == 0:
            self.messages_buffer.clear()

        self.messages_buffer.append(unpacked_data[6:10])

        if len(self.messages_buffer) == chunk_count:
            self.receive_queue.put(command)
            self.receive_queue.put(data_type)
            self.receive_queue.put(self.messages_buffer.copy())
            self.receive_message_flag.set()

        logging.debug("Handle response finished.")


class SerialThread(Thread):
    def __init__(self, port, log_file, baud_rate=9600):
        """
        Initializes a thread that 'sits' on given device.

        Parameters
        ----------
        port : str
            com port name or path
        log_file : str
            file name to for logging
        baud_rate : int, optional
            serial port setting
        """
        super(SerialThread, self).__init__()
        self.setDaemon(True)
        self.port = port
        logging.basicConfig(filename=log_file,
                            format='%(asctime)s - {} - %(levelname)s - '
                                   '%(message)s'.format(self.port),
                            level=logging.INFO)
        logging.info('Serial thread for device %s has been initialized.', port)
        self._stopped = False
        self.device = Serial(port, baud_rate)
        logging.info("Started serial port with baud rate set: %s", baud_rate)
        self.device.read_all()
        self.sender = Sender(self.device)
        self.receiver = Receiver(self.device)

    def run(self):
        """ Starts the thread loop. """
        super(SerialThread, self).run()
        while not self._stopped:
            if self.sender.send_message_flag.is_set():
                self.sender.write_to_device()

            if self.device.in_waiting >= Message.length:
                self.receiver.handle_response()

            if self.receiver.receive_message_flag.is_set():
                self.receiver.receive_message()
            sleep(0.001)

    def send(self, message):
        self.sender.send_queue.put(message)
        self.sender.send_message_flag.set()
