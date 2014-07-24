#!/usr/bin/env python

import os
import re
import sys

symbols = [
    ('id', '[A-Za-z]+', 0),
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

        end = self.position
        token = None
        if self.position < len(self.source):
            for tipo, pattern, lbp in symbols:
                text = self.source[self.position:]
                match = re.match(pattern, self.source[self.position:])
                length = len(match.group(0)) if match else 0
                if length > 0 or (token != None and lbp > token[2] and self.position + length == end):
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

def main(args):
    scanner = Scanner(' '.join(args) )
    t1 = scanner.LookAhead(1)
    print t1
    t2 = scanner.Consume()
    print t2
    t3 = scanner.Consume()
    print t3
    

if __name__ == '__main__':
    main(sys.argv[1:])
