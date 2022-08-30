import mysql.connector

base = "SERVER"

mydb = mysql.connector.connect(
  host="",
  user="",
  password="",
  database=""
)

mycursor = mydb.cursor()

print("Liste des serveurs actifs :")


mycursor.execute("SELECT DISTINCT nom, ip, mac FROM %s WHERE DATEDIFF(NOW(), `date`) < 1 ;" %(base))

myresult = mycursor.fetchall()
nb = 0
for x in myresult:
  nb += 1
  print(x)

print("Nombre :", nb)

print("----------\n")

print("Serveur off depuis > 30j < 1j :")

mycursor.execute("SELECT DISTINCT NOM, MAC, MAX(DATE) AS DATE FROM %s WHERE DATEDIFF(NOW(), `date`) > 0 AND DATEDIFF(NOW(), `date`) < 30 AND MAC NOT IN( SELECT DISTINCT mac FROM %s WHERE DATEDIFF(NOW(), `date`) < 1) GROUP BY MAC;" %(base, base))

myresult = mycursor.fetchall()
nb = 0
for x in myresult:
  nb += 1
  print(x)

print("Nombre :", nb)

print("----------\n")

print("Serveur avec un probleme de temps :")

mycursor.execute("SELECT nom, TIMESTAMPDIFF(MINUTE, `date`, datesys) FROM %s WHERE DATEDIFF(NOW(), `date`) < 1 AND (TIMESTAMPDIFF(MINUTE, `date`, datesys) <= -1 OR TIMESTAMPDIFF(MINUTE, `date`, datesys) >= 1) GROUP BY nom ;" %(base))

myresult = mycursor.fetchall()

nb = 0
for x in myresult:
  nb += 1
  print(x)

print("Nombre :", nb)

print("----------\n")

print("Serveur avec un proble de doublon :")

mycursor.execute("SELECT DISTINCT nom, ip, mac FROM %s t1 WHERE EXISTS ( SELECT DISTINCT nom, ip, mac FROM %s t2 WHERE DATEDIFF(NOW(), `date`) < 1 AND t1.ID <> t2.ID AND (t1.mac <> t2.mac OR t1.ip <> t2.ip) AND (t1.nom = t2.nom OR t1.ip = t2.ip OR t1.mac = t2.mac) );" % (base, base))

myresult = mycursor.fetchall()

nb = 0
for x in myresult:
  nb += 1
  print(x)

print("Nombre :", nb)

print("----------\n")
