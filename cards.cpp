/* Greatest thanks to Bence Magyar for the "Deck of Cards" library.
*  
*/

#include <queue>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

using namespace std;

/* 
* Class Card:  Each card has an enumerated suit and value.  For this exercise, 
* assume that Ace has value=1, J=11, Q=12, and K=13.  This class could be 
* sub-classed to implement other games.  For example, BlackJackCard : public Card, 
* where Ace can have value of 1 or 11. 
*/
class Card{

  public: 

/*    Card& operator=(Card& ins){
        Card temp;

        temp.value=ins.value;
        temp.suit=ins.suit;
        return temp;
    }*/

    enum Suit{ 
      DIAMONDS, SPADES, CLUBS, HEARTS
    };

    Card(){
        suit={};
        value={};
    }
    
    Card(short v, Suit s){
      suit = s;
      value = v;
    };
  
    short getValue() { return value; };
    Suit getSuit()  { return suit; };

    static char enum2string(Suit s){
       switch(s){
          case HEARTS:
            return 'H';
          case CLUBS:
            return 'C';
          case SPADES:
            return 'S';
          case DIAMONDS:
            return 'D';
          default:
            return 'X';
          
       }
    };

  private:
    Suit suit;
    short value;

};
/*
* Class 'Dealer'.  A dealer is given a deck of cards can be asked
* to shuffle the deck or deal one card.
*/


class Dealer{

    /*
     * In this implementation, the dealer is a single entity and each
     * player has an attribute defining whether they take the role
     * of dealer.
     * */

  public:
   
    Dealer(deque<Card> cards){
      deck = cards;
    };
    
    /* Method shuffle():  This method takes the remaining cards in the deck and 
    *  randomizes them in 0(n) time:  For a deck of n cards, we select a random card 
    *  and place it in the back of the deck.  Repeat n-1 times.  The permutation is 
    *  completed "in-place" and has no additional space requirement.
    */
    const short top_val(){
        return deck.front().getValue();
    }

    void shuffle(){

      // seed the random-number generator
      srand( time(NULL));
      for (int i = deck.size()-1; i > 0; i--){
        // generate a random number in [0,i]
        int j =  rand() % i;
        Card tmp = deck[i];
        deck[i] = deck[j];
        deck[j] = tmp;
      }
      
    };
  
    /* Method deal_one_card(): This method deals the top card in the deck.  
    *  This method could be overridden in a subclass if we wanted to change the deal 
    *  logic: For example, deal the bottom card, deal a random card, etc...
    *  Returns a null object if the deck is empty
    */ 
    Card& deal_one_card(){
      if( !deck.empty()){
        Card card = deck.front();
        deck.pop_front();
        return card;
      }
      else throw "Dealer: Deck is empty";

    };
    
    /* Method:  show_cards():  This method is used for printing out the current
    *  deck held by the dealer */ 
    void show_cards(){
      deque<Card>::iterator it = deck.begin();
      
      while(it != deck.end()){
         printf("%d,%c|", it->getValue(),Card::enum2string(it->getSuit()));
         it++;
      }
      printf("\n\n"); 
    }

    /* Method: deck_size(): Returns the number of cards left in the dealer's deck
    */
    int deck_size(){ return deck.size(); }; 

  private:
     deque<Card> deck;
};

class Player{
    //the player class takes an identity integer from 1:n.
    int id;

    Card hand;
    bool is_dealer = false;
    short points=1;
public:
    void remove_points(){
        points--;
    }
    short get_points(){
        return points;
    }
    void get_id(){
        std::cout<<id<<"\n";
    }
    int show_id(){
        return id;
    }

    Player(int n){
        id=n;
    }
    //I will implement a circular linked list for gameplay.

    void takeHand(Card& ins){
        hand = ins;
    }

    void view_card(){
        std::cout<<hand.getValue()<<" of "<<hand.enum2string(hand.getSuit());
    }
    Card& get_card(){
        return hand;
    }

    bool swap_neighbor(Player& neighbor){
        if(neighbor.hand.getValue() != 13){
            Card temp;
            temp=hand;
            hand=neighbor.hand;
            neighbor.hand=temp;
            //swaps current player's card with the neighbors'
            return true;
        }
        std::cout<<"\n**I HAVE A KING**\n";
        return false;
    }

    void assign_dealer(){
        is_dealer= true;
    }

    bool dealer_this() const{
        return is_dealer;
    }


};
deque<Card> init_deck(){
    // empty deck
    deque<Card> cards;

    // add the hearts...
    for(short v=1;v<=13;v++){
        Card c(v,Card::HEARTS);
        cards.push_back(c);
    }
    // add the spades...
    for(short v=1;v<=13;v++){
        Card c(v,Card::SPADES);
        cards.push_back(c);
    }
    // add the diamonds...
    for(short v=1;v<=13;v++){
        Card c(v,Card::DIAMONDS);
        cards.push_back(c);
    }
    // add the clubs...
    for(short v=1;v<=13;v++){
        Card c(v,Card::CLUBS);
        cards.push_back(c);
    }
    return cards;
}
class Game{
    //ranter-go-round implementation
    vector<Player> table;
    deque<Card> discard;
    vector<pair<short,Card>> round;
    deque<Card> deck;
    int dealer = 0;

public:
    short current_sum(){
        short sum=0;
        for(auto i : table){
            sum += i.get_points();
        }
        return sum;
    }

    int find_index(int id){
        int k = 0;
        for(auto i : table){
            k++;
            if(i.show_id()==id){
                return k;
            }
        }
    }
    //this is the start of our simulation
    void play(int players){
        if(players<2){
            return;
        }

        table.clear();
        for(int i=1; i<=players;i++){
            table.push_back(Player(i));
        }
        table.at(dealer).assign_dealer();
        /*
         * this function adds new players to the table
         * and sets the first player as the dealer
         * */

        deck=init_deck();
        Dealer var =Dealer(deck);
        var.shuffle();
        while(current_sum()>1){
            for(int i = 0; i<=players-1;i++){
                table.at(i).takeHand(var.deal_one_card());
            }
            //O(n)


            //Now that everyone has a card it is time to start the game.
            //How we structure the game depends on how we perform our analysis.

            for(int i =1;i<=players;i++){
                bool p_k;
                while(true){
                    std::cout<<"\n\n\ndealer is"<<((dealer)%(players))+1<<"\n\n\n";
                    std::cout<<(dealer+i)%players+1;
                    if((dealer+i)%players+1==((dealer)%(players))+1){
                        try {
                            std::cout << "\n\n$dealer$\n\n";
                            std::cout << "\nplayer " << table.at((dealer + i) % players).show_id() << ":\n";
                            std::cout << "your card is: ";
                            table.at((dealer + i) % players).view_card();
                            std::cout << "\n pass(0) or keep(1)?";
                            std::cin >> p_k;
                            if (cin.fail()) {
                                std::cout << "ERROR";
                                cin.clear();
                                cin.ignore();
                                continue;
                            }

                            if (p_k) {
                                //discard.push_back(table.at(i-1).get_card());
                                //continue;
                            }
                            if (!p_k) {
                                //discard.push_back(table.at(i-1).get_card());
                                if (var.top_val() != 13) {
                                    table.at((dealer + i) % players).takeHand(var.deal_one_card());
                                    std::cout << "\nyour new card is: ";
                                    table.at((dealer + i) % players).view_card();
                                    //std::cout<<"\n\ntableend at:"<<(dealer+i)%players<<"\n";

                                    //std::cout<< "\n";
                                } else {
                                    std::cout << "THE DECK TOP WAS A KING" << std::endl;
                                }
                            }
                            break;
                        }
                        catch(std::out_of_range){
                            break;
                        }
                    }
                    else {
                        try {
                            std::cout << "\nplayer " << table.at((dealer + i) % players).show_id() << ":\n";
                            std::cout << "your card is:";
                            table.at((dealer + i) % players).view_card();
                            std::cout << " \npass(0) or keep(1)?";
                            std::cin >> p_k;
                            if (cin.fail()) {
                                std::cout << "ERROR";
                                cin.clear();
                                cin.ignore();
                                continue;
                            }
                            if (!p_k) {
                                table.at((dealer + i + 1) % players).get_id();
                                table.at((dealer + i + 1) % players).view_card();
                                std::cout << "\nabove is next before";
                                table.at((dealer + i) % players).swap_neighbor(table.at((dealer + i + 1) % players));
                                if (table.at((dealer + i + 1) % players).get_card().getValue() != 13) {
                                    std::cout << "\nyour new card is: ";
                                    table.at((dealer + i) % players).view_card();
                                    break;
                                }
                                std::cout << "your neighbor had a king";

                                /*std::cout<<"\n\ntable at:"<<(dealer+i)%players<<"\n";
                                std::cout<<"\n\nswapping w/ ";
                                std::cout<<"table at:"<<(dealer+i+1)%players<<"\n";
                                std::cout<<(dealer+i+1);
                                std::cout<<"\n\n\n";*/

                                /* table.at((dealer+i+1)%players).get_id();
                                 table.at((dealer+i+1)%players).view_card();*/
                            }
                            break;
                        }
                        catch(std::out_of_range){
                            break;
                        }
                    }
                }

                std::cout<<boolalpha<<p_k;

            }
            //after we've finished a round
            std::cout<<"\n\n\nend of round\n";
            for(auto i:round){
                discard.push_back(i.second);
            }
            short round_min=table.at(0).get_card().getValue();
            for(auto j: table){
                if(j.get_card().getValue()<round_min){
                    round_min=j.get_card().getValue();
                }
            }

            for(auto i : table){
                if(i.get_card().getValue()==round_min){
                    i.remove_points();
                }
                if(i.get_points()==0){
                    std::cout<<"player "<<i.show_id()<<" was removed.";
                    players--;
                    table.erase(table.begin()+ find_index(i.show_id())-1);
                }
            }
            std::cout<<"round minimum was: "<<round_min<<std::endl;


            std::cout<<deck.size()<<"=decksize\n";
            std::cout<<discard.size();
            dealer++;
        }
        std::cout<<"end of game";
        //delete var;
    }
};

//  'main' test driver for card and dealer classes
int main(){
    Game first;

    first.play(5);
    return 0;
}



