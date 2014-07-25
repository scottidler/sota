#!/usr/bin/env python

import os
import re
import sys

symbols = [
    ('eof', '\0', 0),
    ('ws', '[ \t]+', 0),
    ('id', '([0-9]+)?[A-Za-z]+([0-9]+)?', 0),
    ('num', '[0-9]+', 0),
    ('add', '\+', 50),
    ('sub', '-', 50),
    ('mul', '\*', 60),
    ('div', '\/', 60),
]

class Scanner(object):
    def __init__(self, source):
        self.position = 0
        self.index = -1
        self.source = source
        self.tokens = []

    def Scan(self):

        token = symbols[0] + (self.position, 0, '\0')
        if self.position < len(self.source):
            end = self.position
            for tipo, pattern, lbp in symbols:
                text = self.source[self.position:]
                string = self.source[self.position:]
                m = re.match(pattern, string)
                match = m.group(0) if m else ''
                length = len(match) if match else 0
                if self.position + length > end or (token != None and lbp > token[2] and self.position + length == end):
                    token = (tipo, pattern, lbp, self.position, length, self.source[self.position:self.position+length])
                    end = self.position + length
        return token

    def LookAhead(self, distance):
        if distance == 0:
            return tokens[self.index]
        while distance >= len(self.tokens) - self.index:
            token = self.Scan()
            self.tokens += token,
            self.position += token[4]

        curr = self.index + distance
        return self.tokens[curr]

    def Consume(self, expected = None, message = None):
        token = self.LookAhead(1)
        self.index += 1
        if expected and token[0] != expected:
            raise Exception(message or '')
        return token

    def Tokenize(self):
        token = self.Consume()
        while token[0] != 'eof':
            token = self.Consume()
        return self.tokens

def main(args):
    scanner = Scanner(' '.join(args) )
    for token in scanner.Tokenize():
        print token
    

if __name__ == '__main__':
    main(sys.argv[1:])
