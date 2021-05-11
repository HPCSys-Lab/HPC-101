gcc laplace_teste.c -O3 -o laplace_teste
./laplace_teste 500 
python3 movie.py --wavefields saidas --iterations 50000 --hop 500 --name animacao
