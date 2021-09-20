import tkinter as tk
from tkinter import *
from tkinter import messagebox
import pandas as pd
import gspread
import smtplib
from email.message import EmailMessage
from datetime import date
root=tk.Tk()
height=root.winfo_screenheight()
width=root.winfo_screenwidth()
root.title("Power Management System")
root.geometry("%dx%d" % (width, height))
root.configure(background='#D9E4EC')


today = date.today()

rate=7.0
c_u=0.0
s_u=0.0
c_u_billed=0.0
s_u_billed=0.0
total_comsumption=0.0
units_this_month=0.0
units_toady=0.0


def total_consumption():
    pass

def get_sheet_data():
    gc = gspread.service_account(filename='power-management-system-325203-d246bf314d20.json')
    sh = gc.open("Power Management System")
    worksheet = sh.get_worksheet(0)
    df = pd.DataFrame(worksheet.get_all_records())
    return df
get_sheet_data()
def update_labels():
    global c_u_billed
    global s_u_billed
    total_consumption_label=tk.Label(root,text=total_comsumption,font=50,bg='#B7CFDC')
    total_consumption_label.place(y=230,x=90)
    units_this_month_label=tk.Label(root,text=units_this_month,font=50,bg='#B7CFDC')
    units_this_month_label.place(y=230,x=405)
    units_toady_label=tk.Label(root,text=units_toady,font=50,bg='#B7CFDC')
    units_toady_label.place(y=230,x=725)
    s0001=tk.Label(root,text="S001",font=18,bg='#B7CFDC')
    s0001.place(y=437,x=70)
    s_units=tk.Label(root,text=str(s_u),font=18,bg='#B7CFDC')
    s_units.place(y=437,x=420)
    s_amount=(s_u-s_u_billed)*rate
    s_amount_lable=tk.Label(root,text=str(int(s_amount)),font=18,bg='#B7CFDC')
    s_amount_lable.place(y=437,x=560)
    c0001=tk.Label(root,text="C001",font=18,bg='#B7CFDC')
    c0001.place(y=465,x=70)
    c_units=tk.Label(root,text=str(c_u),font=18,bg='#B7CFDC')
    c_units.place(y=465,x=420)
    c_amount=(c_u-c_u_billed)*rate
    c_amount_lable=tk.Label(root,text=str(int(c_amount)),font=18,bg='#B7CFDC')
    c_amount_lable.place(y=465,x=560)
    t_amount=c_amount+s_amount
    t_amount_label=tk.Label(root,text=str(int(t_amount)),font=18,bg='#B7CFDC')
    t_amount_label.place(y=660,x=560)
    total_bill_due=tk.Label(root,text=int(t_amount),font=50,bg='#B7CFDC')
    total_bill_due.place(y=230,x=1030)
    t_units=c_u+s_u
    total_units=tk.Label(root,text=str(int(t_units)),font=18,bg='#B7CFDC')
    total_units.place(y=660,x=420)

def change_rate_button():
    messagebox.showerror("Prohibited Action", "Error!!!\nRate is not allwoed to change.")

def generate_bill_button():
    global c_u_billed
    global s_u_billed
    def create_message(units_to_be_billed):
        msg = EmailMessage()
        msg.set_content('Dear User,\nYour electricity biil for this month is '+str(units_to_be_billed*rate)+".Charged at the rate of "+str(rate)+". and Units billed this month are "+str(units_to_be_billed)+'.')
        msg['Subject'] = 'Power Management System'
        msg['From'] = "ue209071.sachin.mech@gmail.com"
        msg['To'] = "skyetw20@gmail.com"
        return msg
    
    server = smtplib.SMTP('64.233.184.108',587)
    server.ehlo()
    server.starttls()
    server.login("ue209071.sachin.mech@gmail.com", "skyetw@ilmi")
    server.send_message(create_message(c_u-c_u_billed))
    c_u_billed=c_u
    server.send_message(create_message(s_u-s_u_billed))
    s_u_billed=s_u
    server.quit

    messagebox.showinfo("Done", "Bill generated.\nE-Mail sent to users.")

def refresh_button():
    global c_u
    global s_u
    global total_comsumption
    global units_this_month
    global units_toady
    c_u+=10
    df=get_sheet_data()
    Name_list = df["User ID"].tolist()
    Unit_list=df["Units"].tolist()
    date_list=df["Date"].tolist()
    date_list.reverse()
    Unit_list.reverse()
    last_s_units=0.0
    Name_list.reverse()
    c_u=Unit_list[int(Name_list.index('C001'))]
    s_u=Unit_list[int(Name_list.index('S001'))]
    total_comsumption=str(c_u+s_u)+' KWh'
    units_this_month=str(c_u+s_u)+' KWh'
    units_toady=str(c_u+s_u)+' KWh'
    update_labels()



img = tk.PhotoImage(file="Toptitle final.png")
label = tk.Label(root, image=img,bd=0)
label.place(y=10,x=27)
img_status = tk.PhotoImage(file="Statusbar1.png")
label1 = tk.Label(root, image=img_status,bd=0)
label1.place(y=120,x=27)
img_con = tk.PhotoImage(file="bodyfinal2.png")
label2 = tk.Label(root, image=img_con,bd=0)
label2.place(y=330,x=27)
change_rate=tk.Button(root,text="Current rate is ₹7/unit. Click Here to change.",command=change_rate_button,font=20,height=3,bg='#274472',fg='#ffffff',width=50,activebackground="#091830",activeforeground="#ffffff")
change_rate.place(y=450,x=780)
#send_notice=tk.Button(root,text="Send Notices",font=20,height=3,bg='#274472',fg='#ffffff',width=25,activebackground="#091830",activeforeground="#ffffff")
#send_notice.place(y=450,x=1040)
generate_bill=tk.Button(root,text="Generate Bills",font=20,height=3,bg='#274472',fg='#ffffff',command=generate_bill_button,width=25,activebackground="#091830",activeforeground="#ffffff")
generate_bill.place(y=550,x=750)
send_notice=tk.Button(root,text="Refresh",font=20,height=3,bg='#274472',fg='#ffffff',command=refresh_button,width=25,activebackground="#091830",activeforeground="#ffffff")
send_notice.place(y=550,x=1040)
credit=tk.Label(root,text='Interface developed by Team No.-11(Ravi Kumar Vimal,Ritvik Gupta,Sachin Kumar) for the project requirement of "Product Re-engineering and Innovation Course" in University Institute of Engineering and Technology,Panjab University-Chandigarh')
credit.place(y=720,x=27)
tk.mainloop()
