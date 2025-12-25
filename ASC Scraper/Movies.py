import json
import re
import os
import requests
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from bs4 import BeautifulSoup
from fpdf import FPDF
import numpy as np 
import pandas as pd

driver = webdriver.Chrome()
driver.get("https://www.imdb.com/search/title/?groups=top_100&sort=user_rating,desc") 

driver.implicitly_wait(5)

for j in range(1,51):
    