#include "iostream"

using namespace std;

int n;

int main(){
	cin >> n;
	
	for(int i = 0; i < n; i++){
		for(int j = i; j < n-1; j++){
			cout << " ";
		}
		for(int j = 0; j <= i*2 ; j++){
			cout << "*";
		}
		cout << "\n";
	}

	for(int i = n-1; i > 0; i--){
		for(int j = 0; j < n-i; j++){
			cout<<" ";
		}
		for(int j = (i*2)-1; j > 0; j--){
			cout<<"*";
		}
		cout<<"\n";	
	}

}
