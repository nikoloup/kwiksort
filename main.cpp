/* 
 * File:   main.cpp
 * Author: Diomedea Exulans
 *
 * Created on July 24, 2017, 4:59 PM
 */

#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>

using namespace std;


bool generate_tournament(bool** tournament, bool* identifier, int number_of_alternatives, int identifier_length){
    
    int row = 1;
    int pos = 0;
    
    while (row < number_of_alternatives && pos < identifier_length){
        for (int i = 0; i <= row-1; i++){
            tournament[row][i] = identifier[pos];
            tournament[i][row] = !identifier[pos];
            pos++;
        }
        row++;
    }
    
    return true;
}

bool KwiksortREC(bool** tournament, int* ranking, int index_start, int index_end, int number_of_alternatives){
    
    //End recursion
    if(index_start == index_end){
        return true;
    }
    
    //Initialize variables
    int die = 0;
    int pivot = 0;
    int left_count = 0;
    int right_count = 0;
    int left_left_index = index_start;
    int left_right_index = 0;
    int right_left_index = 0;
    int right_right_index = index_end;
    int pivot_cur_index = 0;
    int pivot_new_index = 0;
    int temp = 0;
    
    //Roll die
    die = rand() % (index_end - index_start + 1);
    pivot_cur_index = index_start + die;
    pivot = ranking[index_start + die];
    
    //Debug
    cout << "Pivot: " << pivot << endl;
    
    //Count left/right population
    for(int i=index_start; i<=index_end; i++){
        if(ranking[i] == pivot){
            continue;
        }
        if(tournament[ranking[i]][pivot] == 1){
            left_count++;
        }
        else{
            right_count++;
        }
    }
    
    //Place pivot in correct position (swap)
    pivot_new_index = index_start + left_count;
    temp = ranking[pivot_new_index];
    ranking[pivot_new_index] = pivot;
    ranking[pivot_cur_index] = temp;
    
    //Update left/right indices
    if(left_count!=0){ //Check for case where pivot ends up first
        left_right_index = pivot_new_index-1;
    }
    else{
        left_right_index = left_left_index;
    }
    if(right_count!=0){
        right_left_index = pivot_new_index+1;
    }
    else{
        right_left_index = right_right_index;
    }
    
    //If we're not in a degenerate case
    if(left_count!=0 && right_count!=0){
        //Swap rankings until all LEFT are before pivot and all RIGHT are after pivot
        int left_iterator = left_left_index;
        int right_iterator = right_left_index;
        bool left_swap = false;
        bool right_swap = false;

        while(left_iterator <= left_right_index && right_iterator <= right_right_index){
            if(!left_swap){
                if(tournament[ranking[left_iterator]][pivot] == 0){
                    left_swap = true;
                }
                else{
                    left_iterator++;
                }
            }
            if(!right_swap){
                if(tournament[ranking[right_iterator]][pivot] == 1){
                    right_swap = true;
                }
                else{
                    right_iterator++;
                }
            }
            if(left_swap && right_swap){
                temp = ranking[left_iterator];
                ranking[left_iterator] = ranking[right_iterator];
                ranking[right_iterator] = temp;
                left_swap = false;
                right_swap = false;
                left_iterator++;
                right_iterator++;
            }
        }  
    }
    
    //Finally, recurse
    KwiksortREC(tournament, ranking, left_left_index, left_right_index, number_of_alternatives);
    KwiksortREC(tournament, ranking, right_left_index, right_right_index, number_of_alternatives);
}

int main(int argc, char** argv) {

    //Declare variables
    int number_of_alternatives = 4;
    int identifier_length = ((number_of_alternatives)*(number_of_alternatives-1))/2;
    bool identifier[] = {0, 0, 0, 1, 0, 0};
    int* ranking = new int[number_of_alternatives];
    
    //Initialize random seed
    srand(time(NULL));
    
    //Build tournament
    bool** tournament = new bool*[number_of_alternatives];
    for(int i=0; i<number_of_alternatives; i++){
        tournament[i] = new bool[number_of_alternatives];
        fill_n(tournament[i], number_of_alternatives, 0);
    }
    generate_tournament(tournament, identifier, number_of_alternatives, identifier_length);
    
    //Initialize ranking
    for(int i=0; i<number_of_alternatives; i++){
        ranking[i] = i;
    }
    
    //Begin Kwiksort
    int index_start = 0;
    int index_end = number_of_alternatives-1;
    KwiksortREC(tournament, ranking, index_start, index_end, number_of_alternatives);
    
    //Output
    cout << "Final ranking: ";
    for(int i=0; i<number_of_alternatives; i++){
        cout << ranking[i] << " ";
    }
    
}

