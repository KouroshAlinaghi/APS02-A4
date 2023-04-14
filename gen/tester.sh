ln -s ../main.cpp mine.cpp 2> /dev/null
touch mine.txt
touch other.txt
g++ -std=c++11 -o mine mine.cpp 2> /dev/null
g++ -std=c++11 -o other other.cpp 2> /dev/null
g++ -std=c++17 -o gen gen.cpp 2> /dev/null
((i = 1)) 2> /dev/null
while diff mine.txt other.txt > /dev/null
do
  echo $i
  ((i++)) 2> /dev/null
  ./gen
  ./mine assets < input.txt > mine.txt
  ./other < input.txt > other.txt
done
code --diff mine.txt other.txt