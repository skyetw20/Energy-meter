import gspread
import pandas as pd

gc = gspread.service_account(filename='power-management-system-325203-d246bf314d20.json')

sh = gc.open("Power Management System")
worksheet = sh.get_worksheet(0)
dataframe = pd.DataFrame(worksheet.get_all_records())
print(dataframe)
