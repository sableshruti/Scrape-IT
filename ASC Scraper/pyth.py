from selenium import webdriver
from selenium.webdriver.chrome.service import Service

# Set the ChromeDriver path
service = Service("/usr/bin/chromedriver")  # Update path if different
driver = webdriver.Chrome(service=service)

# Test navigation
driver.get("https://www.google.com")
print("Title:", driver.title)

driver.quit()

