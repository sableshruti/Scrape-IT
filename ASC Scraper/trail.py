import json
import re
import os
import requests
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from bs4 import BeautifulSoup
from fpdf import FPDF

def download_pdf(url, save_path):
    # Send a GET request to the webpage
    response = requests.get(url)

    with open(save_path, 'wb') as file: 
        file.write(response.content)
    # Check if the request was successful
    # if response.status_code == 200:
        # Parse the HTML content
    #     print("Failed to retrieve the webpage")

    # else:
    #     print("Failed to retrieve the webpage")

# URL of the webpage containing the PDFs
# webpage_url = 'https://example.com/path/to/webpage-with-pdfs'

# Path to save the downloaded PDFs
# save_directory = './downloaded_pdfs'

# Ensure the save directory exists
# os.makedirs(save_directory, exist_ok=True)

# Call the function to download PDFs


def remove(string):
    pattern = re.compile(r"\\s+")
    return re.sub(pattern, "", string)

def writeToCPPFile(path, fileName, data):
    filePathNameWExt = "./" + path + "/" + fileName 
    file=open(filePathNameWExt, "w", encoding="utf-8")
    file.write(data)
    file.close

driver = webdriver.Chrome()  # Ensure ChromeDriver is installed
driver.get("https://www.cse.iitb.ac.in/~cs101/lectures.html") 

driver.implicitly_wait(5)

for j in range(2, 30):
        name= driver.find_element(By.XPATH, "/html/body/div[2]/div[2]/div/div/table/tbody/tr[" + str(j)+"]/td[3]/a[1]")
        os.makedirs("./files/"+str(name.text), exist_ok=True)
        # driver.back()
        path1 = "/html/body/div[2]/div[2]/div/div/table/tbody/tr[" + str(j)
        path3 = "]/td[4]/a["
        for k in range(1,15):
            path = path1 + path3 + str(k)+"]"  # for course_code field

            driver.switch_to.default_content()

            driver.implicitly_wait(2)
            try:
                corse = driver.find_element(By.XPATH, path)
            except:
                break
            course_code = corse.text
            course_code = remove(course_code)
            corse.click()

            try:
                course_name = driver.find_element(By.XPATH, "/html/body/pre")
            except:
                continue
            driver.implicitly_wait(5)
            data=course_name.text
            writeToCPPFile("./files/"+str(name.text), course_code, data.replace("\n",'\n'))
            
            driver.back()
        try:
            pdf = driver.find_element(By.XPATH, "/html/body/div[2]/div[2]/div/div/table/tbody/tr[" + str(j)+"]/td[3]/a[2]")
        except:
            break
        # name= driver.find_element(By.XPATH, "/html/body/div[2]/div[2]/div/div/table/tbody/tr[" + str(j)+"]/td[3]/a[1]")
        pdff = FPDF()
        pdff.output("./files/"+str(name.text)+"/slides"+".pdf")
        download_pdf(pdf.get_attribute('href'),"./files/"+str(name.text)+"/slides"+".pdf")

driver.back()

print("Done")
