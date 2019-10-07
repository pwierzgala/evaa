class Message:

    def __init__(self):

        self.device_id = None
        self.plant_id = None
        self.number_of_chunks = None
        self.chunk_index = None
        self.command = None
        self.data_type = None
        self.data = None
        self.crc8 = None

    def convert_to_message(self, data):
        """

        :param data: data to create a class
        :type data: str or bytes or bytearray
        """
        if isinstance(data, str) or isinstance(data, bytes) or isinstance(
                data, bytearray):
            if len(data) == 11:
                data = bytes(data)
                self.device_id = data[0]
                self.plant_id = data[1]
                self.number_of_chunks = data[2]
                self.chunk_index = data[3]
                self.command = data[4]
                self.data_type = data[5]
                self.data = int.from_bytes(data[6:10], 'little')
                self.crc8 = data[10]
                return self

            else:
                raise ValueError("Data argument should have a length of 12 "
                                 "characters (bytes). Data: {}\n Length:{}"
                                 .format(data, len(data)))
        else:
            raise TypeError("Incorrect data type.")
