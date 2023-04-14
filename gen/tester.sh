ln -s ../main.cpp mine.cpp
touch mine.txt
touch other.txt
g++ -std=c++11 -o mine mine.cpp
g++ -std=c++11 -o other other.cpp
g++ -std=c++17 -o gen gen.cpp
(i = 1)
while diff mine.txt other.txt
do
  echo $i
  ((i++))
  ./gen
  ./mine assets < input.txt > mine.txt
  ./other < input.txt > other.txt
done
rm mine.txt
rm other.txt
