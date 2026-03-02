#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <cmath>

using namespace std;

class QLearningAgent{
    public: 
        int NUM_STATES;
        int NUM_ACTIONS = 2;
        int GOAL_STATE;
        
        double learning_rate; // ALPHA
        double discount_factor; // GAMMA 
        double epsilon; // & value

        vector<vector<double>> N; // N time a State was queried
        vector<vector<double>> Q; // State | Action
    
        QLearningAgent(
            int num_states, 
            int num_actions,
            int goal_state,
            double learning_rate,
            double discount_factor,
            double epsilon
        )
            : NUM_STATES(num_states),
            NUM_ACTIONS(num_actions),
            GOAL_STATE(goal_state),
            learning_rate(learning_rate),
            discount_factor(discount_factor),
            epsilon(epsilon),
            N(num_states, std::vector<double>(num_actions, 0.0)),
            Q(num_states, std::vector<double>(num_actions, 0.0))
        {

            std::cout << "\n Parâmetros:\n" << endl;
            std::cout << "Número de estados: " << this->NUM_STATES << endl;
            std::cout << "Número de ações: " << this->NUM_ACTIONS << endl;
            std::cout << "Estado objetivo: " << this->GOAL_STATE << endl;
            std::cout << "Estado inicial (fixo): " << this->Q_fixed_initial_state() << endl;
            std::cout << "Taxa de aprendizado (ALPHA): " << this->learning_rate << endl;
            std::cout << "Fator de desconto (GAMMA): " << this->discount_factor << endl;
            std::cout << "Epsilon (&): " << this->epsilon << endl;
        }

        vector<vector<double>> learn(int epochs = 100000){

            srand(time(0));

            for(double ep = 0.0; ep < epochs; ep++){
                double epsilon_cp = this->epsilon / (ep + 1.0);

                int state = this->Q_state();

                while (state !=  this->GOAL_STATE){
                    int action = this->chooseAction(state, epsilon_cp);
                    this->N[state][action] += 1;
                    
                    int nextState = state;

                    if(action == 0 && state > 0){
                        nextState--;
                    } else if (action == 1 && state < (this->NUM_STATES - 1)) {
                        nextState++;
                    }

                    double reward = (nextState == GOAL_STATE) ? 1.0 : -0.01;
                    double maxQ = *max_element(this->Q[nextState].begin(), this->Q[nextState].end());

                    learning_rate = 1.0 / this->N[state][action];
                    
                    Q[state][action] += learning_rate * (reward + discount_factor * maxQ - this->Q[state][action]);
                    state = nextState;
                }
            }

            return this->Q;
        }

        void display_Q_Table(){
            cout << "\n Q-Table aprendida:\n\n";
            for (int s = 0; s < this->NUM_STATES; s++) {
                cout << "Estado " << s << ": ";
                for (int a = 0; a < this->NUM_ACTIONS; a++) {
                    cout << this->Q[s][a] << " ";
                }
                cout <<  endl;
            }

            cout << "\n\n" << endl;
        }

    private:
        int chooseAction(int state, double epsilon_decayed) {

            if(((double)rand() / RAND_MAX) < epsilon_decayed){
                return rand() % this->NUM_ACTIONS;
            } else {
                return max_element(this->Q[state].begin(), this->Q[state].end()) - this->Q[state].begin();
            }
        }

        int Q_state() {
            return rand() % this->NUM_STATES;
        }

        int Q_fixed_initial_state(){
            return std::ceil(this->Q.size() / this->Q[0].size());
        }
};

int main() {
    std::cout << "\n\nTeste de modelo simples de Aprendizado por Reforço (Q-learning)" << std::endl;

    QLearningAgent model(
        10,
        2,
        3,
        0.1,
        0.9,
        1
    );

    vector<vector<double>> q_table = model.learn();
    model.display_Q_Table();

    return 0;
}