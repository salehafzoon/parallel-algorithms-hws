
import matplotlib.pyplot as plt


def plotSpeedUPs(speedups):

    plt.plot(list(range(1,len(speedups)+1)), speedups, '--',color="blue",
             label='algorithm progress', linewidth=1.5)
    plt.xlabel('threads number')
    plt.ylabel('speed up')
    plt.show()

if __name__ == '__main__':
    
    speedups=[]
    with open('speedups.txt') as file:
        for line in file: 
            line = line.strip() #or some other preprocessing
            speedups.append(float(line))
    
    print(speedups)
    plotSpeedUPs(speedups)
    