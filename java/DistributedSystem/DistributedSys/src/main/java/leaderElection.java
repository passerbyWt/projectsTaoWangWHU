import org.apache.zookeeper.WatchedEvent;
import org.apache.zookeeper.Watcher;
import org.apache.zookeeper.ZooKeeper;
import org.apache.zookeeper.*;

import java.io.IOException;
import java.util.Collections;
import java.util.List;

public class leaderElection implements Watcher{
    private static String zooKeeperAddress="localhost:2181";
    private static int timeOut=3000;
    private ZooKeeper zooKeeper;
    private String nameSpace="/";
    private String curNode;


    public static void main(String[] args) throws IOException, KeeperException,InterruptedException{
        leaderElection leaderEle=new leaderElection();
        leaderEle.connect2ZooKeeper();
        leaderEle.volunteer4Leader();
        leaderEle.electLeader();
        leaderEle.run();
        leaderEle.close();



    }

    private void electLeader() throws KeeperException,InterruptedException{
        List<String> childs=zooKeeper.getChildren(nameSpace,false);
        Collections.sort(childs);
        String leader=childs.get(0);
        if(leader.equals(curNode)){
            System.out.println("I'm the leader");
            return;
        }
        System.out.println("I'm: "+curNode);
        System.out.println("I'm not the leader");
        System.out.println("The leader is:"+leader);



    }

    private void volunteer4Leader() throws InterruptedException,KeeperException{
//        String preFix=nameSpace+"/c_";
        String preFix="/c_";
        String fullPath=zooKeeper.create(preFix,new byte[] {},ZooDefs.Ids.OPEN_ACL_UNSAFE,CreateMode.EPHEMERAL_SEQUENTIAL);
        System.out.println("Znode name:"+fullPath);
//        curNode=fullPath.replace(nameSpace+"/","");
        curNode=fullPath.replace("/","");

    }

    private void close() throws InterruptedException{
        zooKeeper.close();
        System.out.println("Close Connection");
    }

    private void connect2ZooKeeper() throws IOException{
        zooKeeper=new ZooKeeper(zooKeeperAddress,timeOut,this);
    }

    public void run() throws InterruptedException{
        synchronized (zooKeeper){
            zooKeeper.wait();
        }
    }






    @Override
    public void process(WatchedEvent watchedEvent) {
        var type =watchedEvent.getType();
        switch (type){
            case None:
                if (watchedEvent.getState()==Event.KeeperState.SyncConnected)
                {
                    System.out.println("Successfully connect to ZooKeeper");

                }else {
                    System.out.println("Disconnected");
                    synchronized (zooKeeper){
                        zooKeeper.notifyAll();
                    }

                }
        }


    }
}
