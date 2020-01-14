#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

bool check(string a, string b) {// check if there's matching address
	int i = 0, j = b.size();
	if (a[0] == '*') i = b.size() - a.size() + 1;// When IP is *
	if (a[a.size() - 1] == '*') j = a.size() - 1 + i;// When port is *
	for (int k = i; k < j; k ++) {
		if (i == 0 && a[k] != b[k]) return false;
		else if (i != 0 && a[k - i + 1] != b[k]) return false;
	}
	return true;
}

int main() {
	ifstream nat, flow;
	nat.open("NAT.txt");
	string res = "", tmp = "", t1 = "", t2 = "";
	vector<pair<string, string> > list;
	char c;
	
	while (nat.get(c)) {// Build a list of NAT matching
		if (c == ',') {
			t1 = tmp;
			tmp = "";
		}
		else if (c == '\n') {
			t2 = tmp;
			if (t1 != "" && t2 != "") {
				pair<string, string> p;
				p.first = t1;
				p.second = t2;
				list.push_back(p);
			}
			tmp = "";
			t1 = "";
			t2 = "";
		}
		else tmp += c;
	}
	// for the last line of the file
	t2 = tmp;
	if (t1 != "" && t2 != "") {
		pair<string, string> p;
		p.first = t1;
		p.second = t2;
		list.push_back(p);
	}
	tmp = "";
	t1 = "";
	t2 = "";
	
	nat.close();
	flow.open("FLOW.txt");
	while (flow.get(c)) {// Check the input flow
		if (c == '\n') {
			bool isChecked = false;
			for (int i = 0; i < list.size(); i ++) {
				if (check(list[i].first, tmp)) {
					res += tmp + " -> " + list[i].second + '\n';
					isChecked = true;
					break;
				}
			}
			if (!isChecked) res += "No nat match for " + tmp + '\n';
			tmp = "";
		}
		else tmp += c;
	}
	// for the last line of the file
	bool isChecked = false;
	for (int i = 0; i < list.size(); i ++) {
		if (check(list[i].first, tmp)) {
			res += tmp + " -> " + list[i].second;
			isChecked = true;
			break;
		}
	}
	if (!isChecked) res += "No nat match for " + tmp;
	
	flow.close();
	cout << res;
	ofstream fout("OUTPUT.txt");
	fout << res;
	fout.close();
	return 0;
}
