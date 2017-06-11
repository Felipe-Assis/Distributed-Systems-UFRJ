public class Element extends Thread{
    public int funcID;
    public int b;
    public double[] array;
    public RemoteFunc threadFunc;
    public double result;

    @Override
    public void run(){
        try {
            switch(funcID){
                case 1:
                    array=threadFunc.log(array, b);
                    break;
                case 2:
                    array=threadFunc.power(array, b);
                    break;
                case 3:
                    array=threadFunc.square(array);
                    break;
                case 4:
                    result=threadFunc.average(array);
                    break;
                case 5:
                    result=threadFunc.larger(array, b);
                    break;
                case 6:
                    result=threadFunc.deviation(array);
                    break;
            }
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }
}
