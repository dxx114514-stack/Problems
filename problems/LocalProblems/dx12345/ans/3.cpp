#include<bits/stdc++.h>
using namespace std;
#define ll long long
#define str string
#define ull unsigned long long
#define vec vector
const ll mod=LLONG_MAX;
#define sysp system("pause");
ll qp(ll d,ll c){
    ll ans=1;
    while(c){
        if(c%2){
            ans=ans*d%mod;
        }
        d=d*d%mod;
        c/=2;
    }
    return ans;
}
struct player{
    int hunger=20,blood=20,pointguard=0,hun=5,died=0;
    double xhd=0.0,point=0.0;
    double getNeedPoint(){
        if(pointguard<=15)return 2*pointguard+7;
        else if(pointguard<=30)return 5*pointguard-38;
        else return 9*pointguard-158;
    }
    void countxhd(){
        hunger-=floor(xhd/4);
        xhd=xhd-floor(xhd/4)*4;
    }
    void countblood(){
        if(blood<=0){
            hunger=20;
            blood=20;
            pointguard=0;
            hun=5;
            died+=1;
            xhd=0;
            point=0;
        }
    }
    void break_(string block){
        xhd+=0.005;
        if(block=="coal_ore"||block=="deepslate_coal_ore")point+=1;
        else if(block=="diamond_ore"||block=="deepslate_diamond_ore")point+=5;
        else if(block=="emerald_ore"||block=="deepslate_emerald_ore")point+=5;
        else if(block=="lapis_ore"||block=="deepslate_lapis_ore")point+=3.5;
        else if(block=="redstone_ore"||block=="deepslate_redstone_ore")point+=3;
        else if(block=="nether_quartz_ore")point+=3.5;
        else if(block=="amethyst_cluster")point+=4;
        countxhd();
    }
    void swim(ll meter){
        xhd+=0.01*meter;
        countxhd();
    }
    void jump(){
        xhd+=0.05;
        countxhd();
    }
    void sprint(ll meter){
        xhd+=0.1*meter;
        countxhd();
    }
    void kill(str mob){
        xhd+=10;
        countxhd();
        if(mob=="Wither")point+=50;
        else if(mob=="Piglin_Brute")point+=20;
        else if(mob=="Baby_Zombie")point+=12;
        else if(mob=="Evoker"||mob=="Blaze")point+=10;
        else if(mob=="Creeper"||mob=="Enderman")point+=5;
        else if(mob=="Slime")point+=4;
        else if(mob=="Endermite")point+=3;
        else if(mob=="Cow")point+=2;
        else if(mob=="Iron_Golem"||mob=="Bat")point+=0;
        if(point>=getNeedPoint()){
            point-=getNeedPoint();
            pointguard++;
        }
    }
    void no_oxygen(ll sec){
        blood-=2*sec;
        if(blood<=0){
            died+=1;
        }
    }
    void atted_by(str mob){
        if(mob=="Warden")blood-=28;
        else if(mob=="Piglin_Brute")blood-=15;
        else if(mob=="Elder_Guardian")blood-=13;
        else if(mob=="Vindicator"||mob=="Ravager")blood-=11;
        else if(mob=="Piglin")blood-=9;
        else if(mob=="Phantom")blood-=8;
        else if(mob=="Enderman")blood-=7;
        else if(mob=="Creeper"||mob=="Evoker"||mob=="Guardian"||mob=="Hoglin"||mob=="Ghast"||mob=="Blaze"||mob=="Magma_Cube"||mob=="Pillager"||mob=="Drowned"||mob=="Witch"||mob=="Zombified_Piglin")blood-=5;
        else if(mob=="Skeleton"||mob=="Stray")blood-=4;
        else if(mob=="Zombie"||mob=="Husk"||mob=="Slime")blood-=3;
        else if(mob=="Spider")blood-=2;
        else if(mob=="Cave_Spider")blood-=1;
    }
};
int main(){
    
    return 0;
}