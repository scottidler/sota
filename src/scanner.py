#!/usr/bin/env python

import os
import re
import sys

symbols = [
    ('eof', '\0', 0, False),
    ('ws', '[ \t]+', 0, True),
    ('id', '([0-9]+)?[A-Za-z]+([0-9]+)?', 0, False),
    ('num', '[0-9]+', 0, False),
    ('add', '\+', 50, False),
    ('sub', '-', 50, False),
    ('mul', '\*', 60, False),
    ('div', '\/', 60, False),
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
            for tipo, pattern, lbp, skip in symbols:
                text = self.source[self.position:]
                string = self.source[self.position:]
                m = re.match(pattern, string)
                match = m.group(0) if m else ''
                length = len(match) if match else 0
                if self.position + length > end or (token != None and lbp > token[2] and self.position + length == end):
                    token = (tipo, pattern, lbp, skip, self.position, length, self.source[self.position:self.position+length])
                    end = self.position + length
        return token

    def LookAhead(self, distance, skips=False):
        token = None
        i = self.index + 1
        while distance:
            if i < len(self.tokens):
                token = self.tokens[i]
            else:
                token = self.Scan()
                self.position += token[5]
                self.tokens += token,
            skip = token[3]
            if skips or token[3] == False: #if skip
                distance -= 1
            i += 1
        return token

    def Consume(self, expected=None, message=None, skips=False):
        token = self.LookAhead(1, skips=skips)
        self.index += 1
        if expected and token[0] != expected:
            raise Exception(message or '')
        return token

    def Tokenize(self, skips=False):
        token = self.Consume(skips=skips)
        while token[0] != 'eof':
            token = self.Consume(skips=skips)
        return [ token for token in self.tokens if not skips and not token[3] ]

def main(args):
    scanner = Scanner(' '.join(args) )
    for token in scanner.Tokenize():
        print token
    

if __name__ == '__main__':
    main(sys.argv[1:])
