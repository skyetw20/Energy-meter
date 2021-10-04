import smtplib
from email.message import EmailMessage
msg = EmailMessage()

msg.set_content('This is my message')
msg['Subject'] = 'Subject'
msg['From'] = "ue209071.sachin.mech@gmail.com"
msg['To'] = "skyetw20@gmail.com"




server = smtplib.SMTP('64.233.184.108',587)
server.ehlo()
server.starttls()


server.login("ue209071.sachin.mech@gmail.com", "*************")
server.send_message(msg)
#server.sendmail("ue209071.sachin.mech@gmail.com","skyetw20@gmail.com","test mail ")
print("Done")
