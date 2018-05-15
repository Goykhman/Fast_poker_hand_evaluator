/********************************************************************************
 
 Test non-transitive property of poker: compare the strengh of the hands
 AKo, JTs, 22
 
 Specifically we select AKo -> AdKh, JTs -> JdTd, 22 -> 2d2h
 
 Assume diamonds=256, hearts=512 (see Deck.h for representation of the cards)
 
 In terms of number encodings (rank plus suit):
 
 Ad -> 2+(1<<8)=2+256=258
 Kh -> 3+(2<<8)=3+512=515
 Jd -> 7+(1<<8)=7+256=263
 Td -> 11+(1<<8)=11+256=267
 2d -> 41+(1<<8)=41+256=297
 2h -> 41+(2<<8)=41+512=553
 
 AKo=(258,515)
 JTs=(263,267)
 22=(297,553)
 
 Round results (flop is transitive, at least for these cards):
 
 Probability of AKo winning over 22 is 0.465443
 Probability of 22 winning over AKo is 0.52799
 
 Probability of AKo winning over JTs is 0.601313
 Probability of JTs winning over AKo is 0.393402
 
 Probability of JTs winning over 22 is 0.524493
 Probability of 22 winning over JTs is 0.460822 
 
 22>AKo>JTs>22
 
********************************************************************************/

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdlib>
#include <random>
#include <time.h>
#include <sstream>
#include <fstream>
#include <unordered_map>

#include "Deck.h"
#include "CheckRank.h"

using namespace std;

class NonTransitive{

public:

    NonTransitive(int n){
        // Number of rounds to play.
        N=n;
        // Inverse order ranking.
        index_to_rank={{12,"2"},{11,"3"},{10,"4"},{9,"5"},{8,"6"},
            {7,"7"},{6,"8"},{5,"9"},{4,"10"},{3,"J"},{2,"Q"},
            {1,"K"},{0,"A"}};
        prime_to_index={{2,0},{3,1},{5,2},{7,3},{11,4},{13,5},
            {17,6},{19,7},{23,8},{29,9},{31,10},{37,11},
            {41,12}};
        AKoWinsOver22={0,0};
        AKoWinsOverJTs={0,0};
        JTsWinsOver22={0,0};
        // possible choices of 5 cards from 7-card set used to find the best
        // 5-card hand which each player can claim composed of its two hole
        // cards and five community cards.
        choices={{1,1,1,1,1,0,0},
                 {1,1,1,1,0,1,0},
                 {1,1,1,0,1,1,0},
                 {1,1,0,1,1,1,0},
                 {1,0,1,1,1,1,0},
                 {0,1,1,1,1,1,0},
                 {1,1,1,1,0,0,1},
                 {1,1,1,0,1,0,1},
                 {1,1,0,1,1,0,1},
                 {1,0,1,1,1,0,1},
                 {0,1,1,1,1,0,1},
                 {1,1,1,0,0,1,1},
                 {1,1,0,1,0,1,1},
                 {1,0,1,1,0,1,1},
                 {0,1,1,1,0,1,1},
                 {1,1,0,0,1,1,1},
                 {1,0,1,0,1,1,1},
                 {0,1,1,0,1,1,1},
                 {1,0,0,1,1,1,1},
                 {0,1,0,1,1,1,1},
                 {0,0,1,1,1,1,1}};
    }

    // Round played with 5 coommunity cards
    void round(){
        // AKoWinsOverJTs
        vector<int> player1={258,515}; // AKo
        vector<int> player2={263,267}; // JTs
        Deck deck1;
        vector<int> community_cards;
        for(int i=0;i<5;++i){
            int c=deck1.deal_card();
            while(c==258||c==515||c==263||c==267)
                c=deck1.deal_card();
            community_cards.push_back(c);
        }
        copy(community_cards.begin(),community_cards.end(),back_inserter(player1));
        copy(community_cards.begin(),community_cards.end(),back_inserter(player2));
        int r1=INT_MAX;
        int r2=INT_MAX;
        for(int j=0;j<choices.size();++j){
            vector<int> hand1;
            vector<int> hand2;
            for(int k=0;k<7;++k){
                if(choices[j][k]==1){
                    hand1.push_back(player1[k]);
                    hand2.push_back(player2[k]);
                }
            }
            int R1=checkrank.findRank(hand1);
            int R2=checkrank.findRank(hand2);
            if(R1<r1)
                r1=R1;
            if(R2<r2)
                r2=R2;
        }
        if(r1<r2)
            AKoWinsOverJTs[0]+=1;
        else if(r2<r1)
            AKoWinsOverJTs[1]+=1;
        // AKoWinsOver22
        player1={258,515}; // AKo
        player2={297,553}; // 22
        Deck deck2;
        community_cards={};
        for(int i=0;i<5;++i){
            int c=deck2.deal_card();
            while(c==258||c==515||c==297||c==553)
                c=deck2.deal_card();
            community_cards.push_back(c);
        }
        copy(community_cards.begin(),community_cards.end(),back_inserter(player1));
        copy(community_cards.begin(),community_cards.end(),back_inserter(player2));
        r1=INT_MAX;
        r2=INT_MAX;
        for(int j=0;j<choices.size();++j){
            vector<int> hand1;
            vector<int> hand2;
            for(int k=0;k<7;++k){
                if(choices[j][k]==1){
                    hand1.push_back(player1[k]);
                    hand2.push_back(player2[k]);
                }
            }
            int R1=checkrank.findRank(hand1);
            int R2=checkrank.findRank(hand2);
            if(R1<r1)
                r1=R1;
            if(R2<r2)
                r2=R2;
        }
        if(r1<r2)
            AKoWinsOver22[0]+=1;
        else if(r2<r1)
            AKoWinsOver22[1]+=1;
        // JTsWinsOver22
        player1={263,267}; // JTs
        player2={297,553}; // 22
        Deck deck3;
        community_cards={};
        for(int i=0;i<5;++i){
            int c=deck3.deal_card();
            while(c==263||c==267||c==297||c==553)
                c=deck3.deal_card();
            community_cards.push_back(c);
        }
        copy(community_cards.begin(),community_cards.end(),back_inserter(player1));
        copy(community_cards.begin(),community_cards.end(),back_inserter(player2));
        r1=INT_MAX;
        r2=INT_MAX;
        for(int j=0;j<choices.size();++j){
            vector<int> hand1;
            vector<int> hand2;
            for(int k=0;k<7;++k){
                if(choices[j][k]==1){
                    hand1.push_back(player1[k]);
                    hand2.push_back(player2[k]);
                }
            }
            int R1=checkrank.findRank(hand1);
            int R2=checkrank.findRank(hand2);
            if(R1<r1)
                r1=R1;
            if(R2<r2)
                r2=R2;
        }
        if(r1<r2)
            JTsWinsOver22[0]+=1;
        else if(r2<r1)
            JTsWinsOver22[1]+=1;
    }
    
    // Round played with only 3 coommunity cards (flop)
    void flop(){
        // AKoWinsOverJTs
        vector<int> player1={258,515}; // AKo
        vector<int> player2={263,267}; // JTs
        Deck deck1;
        vector<int> community_cards;
        for(int i=0;i<3;++i){
            int c=deck1.deal_card();
            while(c==258||c==515||c==263||c==267)
                c=deck1.deal_card();
            community_cards.push_back(c);
        }
        copy(community_cards.begin(),community_cards.end(),back_inserter(player1));
        copy(community_cards.begin(),community_cards.end(),back_inserter(player2));
        int r1=checkrank.findRank(player1);
        int r2=checkrank.findRank(player2);
        if(r1<r2)
            AKoWinsOverJTs[0]+=1;
        else if(r2<r1)
            AKoWinsOverJTs[1]+=1;
        // AKoWinsOver22
        player1={258,515}; // AKo
        player2={297,553}; // 22
        Deck deck2;
        community_cards={};
        for(int i=0;i<3;++i){
            int c=deck2.deal_card();
            while(c==258||c==515||c==297||c==553)
                c=deck2.deal_card();
            community_cards.push_back(c);
        }
        copy(community_cards.begin(),community_cards.end(),back_inserter(player1));
        copy(community_cards.begin(),community_cards.end(),back_inserter(player2));
        r1=checkrank.findRank(player1);
        r2=checkrank.findRank(player2);
        if(r1<r2)
            AKoWinsOver22[0]+=1;
        else if(r2<r1)
            AKoWinsOver22[1]+=1;
        // JTsWinsOver22
        player1={263,267}; // JTs
        player2={297,553}; // 22
        Deck deck3;
        community_cards={};
        for(int i=0;i<3;++i){
            int c=deck3.deal_card();
            while(c==263||c==267||c==297||c==553)
                c=deck3.deal_card();
            community_cards.push_back(c);
        }
        copy(community_cards.begin(),community_cards.end(),back_inserter(player1));
        copy(community_cards.begin(),community_cards.end(),back_inserter(player2));
        r1=checkrank.findRank(player1);
        r2=checkrank.findRank(player2);
        if(r1<r2)
            JTsWinsOver22[0]+=1;
        else if(r2<r1)
            JTsWinsOver22[1]+=1;
    }

    
    void play_rounds(){
        for(int i=0;i<N;++i){
            if(i%100000==0)
                cout << "i=" << i << endl;
            round();
        }
        double p1=AKoWinsOver22[0]/N;
        double p2=AKoWinsOver22[1]/N;
        double p3=AKoWinsOverJTs[0]/N;
        double p4=AKoWinsOverJTs[1]/N;
        double p5=JTsWinsOver22[0]/N;
        double p6=JTsWinsOver22[1]/N;
        cout << "Round results:" << endl;
        cout << "Probability of AKo winning over 22 is " << p1 << " "
        << "Probability of 22 winning over AKo is " << p2 << " " << endl;
        cout << "Probability of AKo winning over JTs is " << p3 << " "
        << "Probability of JTs winning over AKo is " << p4 << " " << endl;
        cout << "Probability of JTs winning over 22 is " << p5 << " "
        << "Probability of 22 winning over JTs is " << p6 << " " << endl;
        cout << endl;
    }
    

    void play_flops(){
        for(int i=0;i<N;++i){
            if(i%100000==0)
                cout << "i=" << i << endl;
            flop();
        }
        double p1=AKoWinsOver22[0]/N;
        double p2=AKoWinsOver22[1]/N;
        double p3=AKoWinsOverJTs[0]/N;
        double p4=AKoWinsOverJTs[1]/N;
        double p5=JTsWinsOver22[0]/N;
        double p6=JTsWinsOver22[1]/N;
        cout << "Flop results:" << endl;
        cout << "Probability of AKo winning over 22 is " << p1 << " "
             << "Probability of 22 winning over AKo is " << p2 << " " << endl;
        cout << "Probability of AKo winning over JTs is " << p3 << " "
             << "Probability of JTs winning over AKo is " << p4 << " " << endl;
        cout << "Probability of JTs winning over 22 is " << p5 << " "
             << "Probability of 22 winning over JTs is " << p6 << " " << endl;
        cout << endl;
    }

private:
    vector<double> AKoWinsOver22;
    vector<double> AKoWinsOverJTs;
    vector<double> JTsWinsOver22;
    int N;
    map<int,string> index_to_rank;
    map<int,int> prime_to_index;
    vector<vector<int>> choices;
    CheckRank checkrank;
};

int main(){
    
    srand(10000*time(NULL));
    
    NonTransitive nontransitive(1000000);
    

    //nontransitive.play_flops();
    
    nontransitive.play_rounds();
    
    return 0;
}

/*
 
 */
