rm abc.txt
rm a.txt
rm e.txt
rm f.txt
rm vlc.sh
touch e.txt
touch f.txt
touch vlc.sh
(ps aux | grep vlc) >>abc.txt 
awk '{print $2}' abc.txt >> a.txt
cut -f 3 -d ' ' a.txt >> a.txt
head -1 a.txt >> f.txt
value=`cat f.txt`
echo "sudo kill -9 $value" >> vlc.sh
chmod +x vlc.sh
bash vlc.sh
