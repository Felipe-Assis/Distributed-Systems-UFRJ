import java.rmi.*;
import java.net.*;
import java.util.Arrays;
import java.util.concurrent.ThreadLocalRandom;

public class RMIClient{

   public static void main(String[] args){
      try {
         System.out.println("Initializing client...");
         String serverURL="rmi://"+args[0]+"/RMIServer";
         RemoteFunc function= (RemoteFunc) Naming.lookup(serverURL);
         System.out.println("Client initialized.");
         //===========================================
         int N = 10000000;
         int K = Integer.parseInt(args[1]);
         int opperation = Integer.parseInt(args[2]);
         Element[] thread = new Element[K];
         double[] num = new double[N];
         double[] temp;
         int b;
         if (args.length > 3){
            b = Integer.parseInt(args[3]);
         }
         else{
            b = 2;
         }
         double res = 0;
         //long begin, end;
         long startTime, estimatedTime, averageTime;
         averageTime = 0;

         //for (int i=0;i<N;i++)
         //    num[i]=(double) ThreadLocalRandom.current().nextInt(1,100);

         //System.out.println(Arrays.toString(num));


        for (int i=0;i<N;i++)
           num[i]=(double) ThreadLocalRandom.current().nextInt(1,100);

        for (int i=0;i<K;i++){
           thread[i]=new Element();
           thread[i].funcID=opperation;
           thread[i].b=b;   
           thread[i].array=Arrays.copyOfRange(num, i*(N/K), i*(N/K)+(N/K));
           thread[i].threadFunc=function;
        }

        //begin=System.nanoTime();
        startTime = System.currentTimeMillis();

        for (int i=0;i<K;i++)
           thread[i].start();
       
        for (int i=0;i<K;i++){
           thread[i].join();
        }

        estimatedTime = System.currentTimeMillis() - startTime;
        System.out.println("Execution time: "+(estimatedTime)+" ms");
        averageTime += estimatedTime;
        
        //Process or join results received from server
        switch(opperation){
           case 1:
           case 2: 
           case 3:
             for (int i=0;i<K;i++)
                 for (int j=0; j<thread[i].array.length; j++)
                     num[j+i*K] = thread[i].array[j];
              break;

           case 4:
              for (int i=0;i<K;i++)
                 res += thread[i].result;
              res = res/num.length;
              break;

           case 5:
              for (int i=0;i<K;i++)
                 res += thread[i].result;
              break;

           case 6:
              for (int i=0;i<K;i++)
                 res += thread[i].result;
              res = Math.sqrt(res/num.length);
              break;
        }

        //end=System.nanoTime();

        //System.out.println("Execution time: "+(end-begin)+" ns");



         System.out.println("Average time: "+averageTime/20+" ms");
         //System.out.println(Arrays.toString(num ));

      }catch(Exception e) {
         System.out.println("Exception:  " + e);
      }
   }
}