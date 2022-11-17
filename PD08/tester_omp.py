from subprocess import Popen, PIPE

iterations = 20
program_path = "./omp -noevalprints"

for n in [1000000, 10000000,100000000]:
  print("OMP, n:",n)
  print("   processors", "  |\t","average(s)")
  print("-------------------------------------")
  for pr_i in [1,2,4,8,12,14,16]:
    avg = 0
    for i in range(iterations):
      p = Popen([program_path], shell=True, stdout=PIPE, stdin=PIPE)
      p.stdin.write((str(pr_i)+"\n").encode())
      p.stdin.write((str(n)+"\n").encode())
      p.stdin.flush()
      result = p.stdout.readline().strip().decode()
      avg += float(result)
    avg = avg / iterations
    print("\t",pr_i, "\t|  ",f'{avg:.20f}')
