from subprocess import Popen, PIPE

iterations = 20

print("   processors", "  |\t","average(s)")
print("-------------------------------------")

for pr_i in [1,2,4,8,12]:
  avg = 0
  program_path = f"mpirun -np {pr_i} ./mpi -noevalprints"
  for i in range(iterations):
    p = Popen([program_path], shell=True, stdout=PIPE, stdin=PIPE)
    p.stdin.write((str(pr_i)+"\n").encode())
    p.stdin.flush()
    result = p.stdout.readline().strip().decode()
    avg += float(result)
  avg = avg / iterations
  print("\t",pr_i, "\t|  ",f'{avg:.20f}')
