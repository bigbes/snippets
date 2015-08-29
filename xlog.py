#!/usr/bin/env python

import io
import json
import msgpack

import crcmod

crc32 = crcmod.mkCrcFun('4812730177', 0)

class LogReaderError(Exception):
    pass


# Snapshot/XLOG reader classs
class LogReader(object):
    _formats = ['SNAP', 'XLOG']
    _inprogress = '.inprogress'
    _marker_row = '\xd5\xba\x0b\xab'
    _marker_eof = '\xd5\x10\xad\xed'
    _irow_size  = 15
#    _buf_size = 4096

    def __init__(self, name):
        self.name = name
        self.io = io.open(name, 'rb')
        self.inprogress = (self._inprogress == name[-len(self._inprogress):])
        self.upck = msgpack.Unpacker()
        while True:
            line = self.io.readline().strip()
            if line in self._formats:
                self.format = line
            elif line.startswith('0'):
                self.format_version = line
            elif line.startswith('Server: '):
                self.server_uuid = line[8:]
            elif line.startswith('VClock: '):
                self.vclock = line[8:]
            elif not line:
                break
            else:
                raise LogReaderError('')

        self.log_begin = self.io.tell()
#        self.buf += self.io.read(self._buf_size)

    def read_unknown_file(self):
        offset = 0
        while True:
            try:
                val = self.upck.next()
                return val
            except StopIteration:
                self.upck.feed(self.io.read(1))
                offset += 1

    def read_unknown_buffer(self, buf):
        offset = 0
        while True:
            try:
                val = self.upck.next()
                return val, buf[offset:]
            except StopIteration:
                if len(buf) <= offset:
                    return None
                self.upck.feed(buf[offset])
                offset += 1

    def next(self):
        offset = 0
        marker = bytearray(self.io.read(4))
        while (marker[-4:] != self._marker_row):
            if (marker[-4:] == self._marker_eof):
                raise StopIteration("EOF")
            marker += self.io.read(1)
            offset += 1
        print "Offset for NEXT is ", offset
        buf = self.io.read(self._irow_size)
        length, buf = self.read_unknown_buffer(buf)
        crc32p, buf = self.read_unknown_buffer(buf)
        crc32n, buf = self.read_unknown_buffer(buf)
        buf = self.io.read(length)
        print "Length of package: ", length
        assert(crc32(buf) == crc32n)
        self.upck.feed(buf)
        header = self.upck.next()
        body = self.upck.next()
        print header
        print body
        return header, body

    def __iter__(self):
        return self

    def formatter(self):
        print "File:", self.name
        print "Format:", self.format
        print "Server UUID:", self.server_uuid
        print "Server VClock:", self.vclock

    def __del__(self):
        self.io.close()

import sys

log = LogReader(sys.argv[1])
log.formatter()
print "=" * 80
for i in log:
    print "=" * 80
