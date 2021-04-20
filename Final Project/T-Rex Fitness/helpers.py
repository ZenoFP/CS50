import os
import requests
import urllib.parse

from flask import redirect, render_template, request, session
from functools import wraps



def push(test):

    if int(test) == 0:
        return 0

    elif int(test) <= 10:
        return 1
    elif int(test) > 10 and int(test) < 16:
        return 2

    elif int(test) > 15 and int(test) < 21:
        return 3

    elif int(test) > 20 and int(test) < 26:
        return 4
    else:
        return 5

def pull(test):
    t = int(test)


    if t == 0:
        return 0

    elif t <= 3:
        return 1
    elif t > 3 and t < 8:
        return 2

    elif t > 7 and t < 11:
        return 3

    elif t > 10 and t < 16:
        return 4
    else:
        return 5

def iso(test):
    t = int(test)

    if t == 0:
        return 0

    elif t < 20:
        return 1
    elif t > 19 and t < 31:
        return 2

    elif t > 30 and t < 70:
        return 3

    elif t > 69 and t < 91:
        return 4
    else:
        return 5


