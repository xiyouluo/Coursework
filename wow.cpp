#include<iostream>
#include<string>
#include<vector>
#include<iomanip>
#include<algorithm>

using namespace std;

const int warriorNum = 5;
int m, n, k, t;

class Weapon{
public:
	string kind; //武器种类
	int id; //武器序号
	long long remainTimes; //剩下的使用次数
	Weapon(int id_): id(id_){
		switch (id_){
			case 0:
				kind = "sword";
				remainTimes = INT64_MAX;
				break;
			case 1:
				kind = "bomb";
				remainTimes = 1;
				break;
			case 2:
				kind = "arrow";
				remainTimes = 2;
				break;
		}
	}
	Weapon(const Weapon& w): kind(w.kind), id(w.id), remainTimes(w.remainTimes){}
	virtual ~Weapon() = default;
	virtual int getAd(int userAd) = 0;
	virtual void updateRemainTimes() = 0;
	virtual Weapon* clone() const = 0;
};
class Sword: public Weapon{
public:
	Sword(): Weapon(0){}
	Sword(const Sword& w): Weapon(w){}
	int getAd(int userAd) override{
		return userAd * 2 / 10;
	}
	void updateRemainTimes() override{}
	Weapon* clone() const override{
        return new Sword(*this);
    }
};
class Bomb: public Weapon{
public:
	Bomb(): Weapon(1){}
	Bomb(const Bomb& w): Weapon(w){}
	int getAd(int userAd) override{
		return userAd * 4 /10;
	}
	void updateRemainTimes() override{
		--remainTimes;
	}
	Weapon* clone() const override{
        return new Bomb(*this);
	}
};
class Arrow: public Weapon{
public:
	Arrow():Weapon(2){}
	Arrow(const Arrow& w): Weapon(w){}
	int getAd(int userAd) override{
		return userAd * 3 / 10;
	}
	void updateRemainTimes() override{
		--remainTimes;
	}
	Weapon* clone() const override{
        return new Arrow(*this);
	}
};

//-------------------------------------------------------------
bool takeCmp(Weapon* w1, Weapon* w2) //先拿的大，排序后放后面
{
	if (w1->id < w2->id)
		return false;
	else if (w1->id == 2 && w2->id == 2){
		if (w1->remainTimes > w2->remainTimes)
			return false;
		else
			return true;
	}
	else
		return true;
}
bool fightCmp(Weapon* w1, Weapon* w2) //序号小的放前面，用过的arrow放前面
{
	if (w1->id == w2->id){
		return w1->remainTimes < w2->remainTimes;
	}
	return w1->id < w2->id;
}

//-------------------------------------------------------------
class Warrior;

class City{
public:
	Warrior* pRedWarrior;
	Warrior* pBlueWarrior;
	int cityID;
	City(): pRedWarrior(nullptr), pBlueWarrior(nullptr){}
	City(Warrior* pRed, Warrior* pBlue){
		pRedWarrior = pRed;
		pBlueWarrior = pBlue;
	}
	~City(){
		cityID = 0;
	}
	void wolfTake(const int& time_);
	void fightAndReport(const int& time_);
	void dragonYell(const int& time_);
	void reportStatus(const int& time_);
};

//-------------------------------------------------------------
class Headquarter;

class Warrior{
friend class Headquarter;
friend class City;
friend void updateCities(Headquarter& r, Headquarter& b, vector<City>& C);
public:
	Headquarter* pHeadquarter;
	string kind; //战士种类
	int id; //战士在该阵营的总编号
	int hp; //health point
	int ad; //攻击力
	int nowCity; //现在所在的城市序号
	static vector<string> kinds; //种类列表（实现种类和数字间的双射）
	static vector<int> hps; //生成战士需要的hp列表
	static vector<int> ads; //战士初始的攻击力列表
	vector<Weapon*> pWeapons;
	
	Warrior(Headquarter* pH, int kindNo);
	virtual ~Warrior(){
		for (auto p : pWeapons)
			delete p;
		pWeapons.clear();
	};
	virtual void decreaseLoyalty() = 0;
	virtual int getLoyalty() = 0;
	void takeDamage(int adversaryAd){
		hp -= adversaryAd;
	}
	bool isAlive(){
		return hp > 0;
	}
	int getWeaponCnt(){
		return pWeapons.size();
	}
	int getEachWeaponCnt(string kindName){
		int temp = 0;
		for (auto w : pWeapons){
			if (w->kind == kindName)
				++temp;
		}
		return temp;
	}
};
class Dragon: public Warrior{
public:
	Dragon(Headquarter* pH, int kindNo): Warrior(pH, kindNo){
		switch(id%3){
			case 0:
				pWeapons.push_back(new Sword());
				break;
			case 1:
				pWeapons.push_back(new Bomb());
				break;
			case 2:
				pWeapons.push_back(new Arrow());
		}
	}
	~Dragon() = default;
	virtual void decreaseLoyalty(){}
	virtual int getLoyalty(){return 0;}
};
class Ninja: public Warrior{
public:
	Ninja(Headquarter* pH, int kindNo): Warrior(pH, kindNo){
		switch(id%3){
			case 0:
				pWeapons.push_back(new Sword());
				break;
			case 1:
				pWeapons.push_back(new Bomb());
				break;
			case 2:
				pWeapons.push_back(new Arrow());
		}
		switch( (id+1)%3 ){
			case 0:
				pWeapons.push_back(new Sword());
				break;
			case 1:
				pWeapons.push_back(new Bomb());
				break;
			case 2:
				pWeapons.push_back(new Arrow());
		}
	}
	~Ninja() = default;
	virtual void decreaseLoyalty(){}
	virtual int getLoyalty(){return 0;}
};
class Iceman: public Warrior{
public:
	Iceman(Headquarter* pH, int kindNo): Warrior(pH, kindNo){
		switch(id%3){
			case 0:
				pWeapons.push_back(new Sword());
				break;
			case 1:
				pWeapons.push_back(new Bomb());
				break;
			case 2:
				pWeapons.push_back(new Arrow());
		}
	}
	~Iceman() = default;
	virtual void decreaseLoyalty(){}
	virtual int getLoyalty(){return 0;}
};
class Lion: public Warrior{
private:
	int loyalty;
public:
	Lion(Headquarter* pH, int kindNo);
	~Lion() = default;
	virtual void decreaseLoyalty(){
		loyalty -= k;
	}
	virtual int getLoyalty(){
		return loyalty;
	}
};
class Wolf: public Warrior{
public:
	Wolf(Headquarter* pH, int kindNo): Warrior(pH, kindNo){}
	~Wolf() = default;
	virtual void decreaseLoyalty(){}
	virtual int getLoyalty(){return 0;}
};

//-------------------------------------------------------------
bool warriorCmp(Warrior* w1, Warrior* w2) //西边的排前面
{
	return w1->nowCity < w2->nowCity;
}

//-------------------------------------------------------------
class Headquarter{
friend class Warrior;
friend class Lion;
friend void updateCities(Headquarter& r, Headquarter& b, vector<City>& C);
private:
	string color; 
	vector<int> order; //生成武士的顺序列表，武士用一个序号表示
	int totalNum; //武士总数
	vector<int> nums; //各类武士的数量列表
	int totalHp; //阵营当前所剩的总hp
	int curMakingIdx; //当前制造序号
public:
	bool isOver;
	bool isTaken;
	vector<Warrior*> pWarriors; //装指针，用多态！！！
	
	Headquarter(string color_, int totalHp_): color(color_), totalHp(totalHp_){
		if (color_ == "red")
			order = {2, 3, 4, 1, 0};
		else if (color_ == "blue")
			order = {3, 0, 1, 2, 4};
		totalNum = 0;
		nums = {0, 0, 0, 0, 0};
		curMakingIdx = 0;
		isOver = false;
		isTaken = false;
	}
	~Headquarter(){
		for (auto pw : pWarriors)
			delete pw;
		pWarriors.clear();
	}
	void produce(int time_);
	void march(int time_){
		//注意Icemman掉血和Lion减忠诚
		sort(pWarriors.begin(), pWarriors.end(), warriorCmp);
		for (auto pw : pWarriors){
			if (pw->kind == "iceman")
				pw->hp -= pw->hp / 10;
			else if (pw->kind == "lion")
				pw->decreaseLoyalty();

			if (color == "red")
				++ pw->nowCity;	
			else if (color == "blue")
				-- pw->nowCity;
		}
	}
	void reportHp(int time_){
		//000:50 100 elements in red headquarter
		cout<<setfill('0')<<setw(3)<<time_<<":50 "<<totalHp<<" elements in "<<color<<" headquarter"<<endl;
	}
};

//-------------------------------------------------------------
vector<string> Warrior::kinds = {"dragon", "ninja", "iceman", "lion", "wolf"};
vector<int> Warrior::hps = {0, 0, 0, 0, 0};
vector<int> Warrior::ads = {0, 0, 0, 0, 0};

//-------------------------------------------------------------
Warrior::Warrior(Headquarter* pH, int kindNo){
	kind = kinds[kindNo];
	pHeadquarter = pH;
	hp = hps[kindNo];
	ad = ads[kindNo];
	id = ++pH->totalNum;
	++pH->nums[kindNo];
	pH->totalHp -= hp;
	//输出:00 blue lion 5 born
	cout<<":00 "<<pH->color<<' '<<kinds[kindNo]<<' '<<id<<" born"<<endl;
	if (pH->color == "red"){
		nowCity = 0;
	}
	else if (pH->color == "blue"){
		nowCity = n+1;
	}
}

Lion::Lion(Headquarter* pH, int kindNo): Warrior(pH, kindNo){
		loyalty = pH->totalHp;
		cout<<"Its loyalty is "<<loyalty<<endl;
		switch(id%3){
			case 0:
				pWeapons.push_back(new Sword());
				break;
			case 1:
				pWeapons.push_back(new Bomb());
				break;
			case 2:
				pWeapons.push_back(new Arrow());
		}
	}

void Headquarter::produce(int time_){
	if (totalHp >= Warrior::hps[order[curMakingIdx]] && !isOver){
		cout<<setfill('0')<<setw(3)<<time_; //001
		switch (order[curMakingIdx]){
			case 0:{
				pWarriors.push_back( new Dragon(this, 0) );
				break;
			}	
			case 1:{
				pWarriors.push_back( new Ninja(this, 1) );
				break;
			}
			case 2:{
				pWarriors.push_back( new Iceman(this, 2) );
				break;
			}	
			case 3:{
				pWarriors.push_back( new Lion(this, 3) );
				break;
			}
			case 4:{
				pWarriors.push_back( new Wolf(this, 4) );
				break;
			}
		}
		curMakingIdx = (curMakingIdx + 1) % warriorNum;
	}
	else
		isOver = true;
}

//-------------------------------------------------------------
void updateCities(Headquarter& r, Headquarter& b, vector<City>& C){
    for (City& city : C){
        city.pRedWarrior = nullptr;
        city.pBlueWarrior = nullptr;
    }

    for (auto pw : r.pWarriors){
        if (pw->nowCity <= n+1)
            C[pw->nowCity].pRedWarrior = pw;
    }
    for (auto pw : b.pWarriors){
        if (pw->nowCity >= 0)
        C[pw->nowCity].pBlueWarrior = pw;
    }
}
void reportMarch(const int& time_, Headquarter& r, Headquarter& b, vector<City>& cities){
	for (auto& c : cities){
		if (c.pRedWarrior){
			if (c.pRedWarrior->nowCity < n+1){
				cout<<setfill('0')<<setw(3)<<time_<<":10 red "<<c.pRedWarrior->kind<<' '<<c.pRedWarrior->id
					<<" marched to city "<<c.pRedWarrior->nowCity<<" with "<<c.pRedWarrior->hp
					<<" elements and force "<<c.pRedWarrior->ad<<endl;
			}
		}
		if (c.pBlueWarrior){
			if (c.pBlueWarrior->nowCity > 0){
				cout<<setfill('0')<<setw(3)<<time_<<":10 blue "<<c.pBlueWarrior->kind<<' '<<c.pBlueWarrior->id
					<<" marched to city "<<c.pBlueWarrior->nowCity<<" with "<<c.pBlueWarrior->hp
					<<" elements and force "<<c.pBlueWarrior->ad<<endl;
			}
		}
	}
}
void lionRun(int time_, Headquarter& r, Headquarter& b){
	for (auto pw : r.pWarriors){
		if (pw->nowCity <= n && pw->kind == "lion"){
			if (pw->getLoyalty() <= 0){
				cout<<setfill('0')<<setw(3)<<":05 red lion "<<pw->id<<" ran away"<<endl;
				//000:05 blue lion 1 ran away
				pw->nowCity = n+2;
			}
		}
	}
	for (auto pw : b.pWarriors){
		if (pw->nowCity > 0 && pw->kind == "lion"){
			if (pw->getLoyalty() <= 0){
				cout<<setfill('0')<<setw(3)<<time_<<":05 blue lion "<<pw->id<<" ran away"<<endl;
				//000:05 blue lion 1 ran away
				pw->nowCity = -1;
			}
		}
	}
}
bool ptrCmp(const Weapon* p1, const Weapon* p2){
	return (p1->id == p2->id && p1->kind == p2->kind && p1->remainTimes == p2->remainTimes);
}
bool isAllSwords(const vector<Weapon*>& p){
	return all_of(p.begin(), p.end(), [](Weapon* pw){return pw->kind == "sword";});
}

//-------------------------------------------------------------
	void City::wolfTake(const int& time_){
		if (pRedWarrior && pBlueWarrior){
			if (pRedWarrior->kind == "wolf" && pBlueWarrior->kind != "wolf" && pRedWarrior->pWeapons.size() < 10){
				int takeID;
				int cnt = 0;
				//把对方的武器排序
				sort(pBlueWarrior->pWeapons.begin(), pBlueWarrior->pWeapons.end(), takeCmp);
				//拿武器，只拿一种！
				if (pBlueWarrior->pWeapons.size())
					takeID = pBlueWarrior->pWeapons.back()->id;
				else
					return;
				while (pRedWarrior->pWeapons.size() < 10 && pBlueWarrior->pWeapons.size()){
					if (pBlueWarrior->pWeapons.back()->id == takeID){
						pRedWarrior->pWeapons.push_back(pBlueWarrior->pWeapons.back());
						pBlueWarrior->pWeapons.pop_back();
						++cnt;
					}
					else
						break;
				}
				//000:35 blue wolf 2 took 3 bomb from red dragon 2 in city 4
				cout<<setfill('0')<<setw(3)<<time_<<":35 red wolf "<<pRedWarrior->id<<" took "<<cnt<<' '
				<<pRedWarrior->pWeapons.back()->kind<<" from blue "<<pBlueWarrior->kind<<' '<<pBlueWarrior->id<<" in city "<<cityID<<endl;
			}
			else if (pBlueWarrior->kind == "wolf" && pRedWarrior->kind != "wolf" && pBlueWarrior->pWeapons.size() < 10){
				int takeID;
				int cnt = 0;
				//把对方的武器排序
				sort(pRedWarrior->pWeapons.begin(), pRedWarrior->pWeapons.end(), takeCmp);
				//拿武器，只拿一种！
				if (pRedWarrior->pWeapons.size())
					takeID = pRedWarrior->pWeapons.back()->id;
				else
					return;
				while (pBlueWarrior->pWeapons.size() < 10 && pRedWarrior->pWeapons.size()){
					if (pRedWarrior->pWeapons.back()->id == takeID){
						pBlueWarrior->pWeapons.push_back(pRedWarrior->pWeapons.back());
						pRedWarrior->pWeapons.pop_back();
						++cnt;
					}
					else
						break;
				}
				//000:35 blue wolf 2 took 3 bomb from red dragon 2 in city 4
				cout<<setfill('0')<<setw(3)<<time_<<":35 blue wolf "<<pBlueWarrior->id<<" took "<<cnt<<' '
				<<pBlueWarrior->pWeapons.back()->kind<<" from red "<<pRedWarrior->kind<<' '<<pRedWarrior->id<<" in city "<<cityID<<endl;
			}
		}
	}
	void City::fightAndReport(const int& time_){
		if (pRedWarrior && pBlueWarrior){
			//武器排序
			sort(pRedWarrior->pWeapons.begin(), pRedWarrior->pWeapons.end(), fightCmp);
			sort(pBlueWarrior->pWeapons.begin(), pBlueWarrior->pWeapons.end(), fightCmp);
			bool aTurn = cityID & 1;//奇数红方先
			int aCurWeapon = 0, bCurWeapon = 0;
			int aPreviousHp = pRedWarrior->hp, bPreviousHp = pBlueWarrior->hp;
			vector<Weapon*> aPreviousPWeapons, bPreviousPWeapons;
			for (auto& pw : pRedWarrior->pWeapons)
    			aPreviousPWeapons.push_back(pw->clone());	
			for (auto& pw : pBlueWarrior->pWeapons)
    			bPreviousPWeapons.push_back(pw->clone());
				
			while (true){
				if (aTurn){
					if (pRedWarrior->getWeaponCnt()){
						//攻击
						int aAd = pRedWarrior->pWeapons[aCurWeapon]->getAd(pRedWarrior->ad);
						pBlueWarrior->takeDamage(aAd);
						if (pRedWarrior->pWeapons[aCurWeapon]->kind == "bomb" && pRedWarrior->kind != "ninja")
							pRedWarrior->takeDamage(aAd / 2);
						//更新武器耐久度，去除废武器
						pRedWarrior->pWeapons[aCurWeapon]->updateRemainTimes();
						pRedWarrior->pWeapons.erase(
    						remove_if(pRedWarrior->pWeapons.begin(), pRedWarrior->pWeapons.end(),
                   			[](const Weapon* w) { return w->remainTimes == 0; }), pRedWarrior->pWeapons.end());

						if (pRedWarrior->getWeaponCnt())
							aCurWeapon = (aCurWeapon + 1) % pRedWarrior->getWeaponCnt();
						//判断战斗结果
						if (!pBlueWarrior->isAlive() && pRedWarrior->isAlive()){
							//000:40 red iceman 1 killed blue lion 12 in city 2 remaining 20 elements
							cout<<setfill('0')<<setw(3)<<time_<<":40 red "<<pRedWarrior->kind<<' '<<pRedWarrior->id<<" killed blue "<<
							pBlueWarrior->kind<<' '<<pBlueWarrior->id<<" in city "<<cityID<<" remaining "<<pRedWarrior->hp<<" elements"<<endl;
							//抢武器
							sort(pBlueWarrior->pWeapons.begin(), pBlueWarrior->pWeapons.end(), takeCmp);
							while (pRedWarrior->pWeapons.size() < 10 && pBlueWarrior->getWeaponCnt()){
								pRedWarrior->pWeapons.push_back(pBlueWarrior->pWeapons.back());
								pBlueWarrior->pWeapons.pop_back();
							}
							//去掉死了的warrior
							pBlueWarrior->nowCity = -1;
							break;
						}
						else if (!pBlueWarrior->isAlive() && !pRedWarrior->isAlive()){
							//000:40 both red iceman 1 and blue lion 12 died in city 2
							cout<<setfill('0')<<setw(3)<<time_<<":40 both red "<<pRedWarrior->kind<<' '<<pRedWarrior->id<<" and blue "<<
							pBlueWarrior->kind<<' '<<pBlueWarrior->id<<" died in city "<<cityID<<endl;
							//去掉死了的warrior
							pRedWarrior->nowCity = n+2;
							pBlueWarrior->nowCity = -1;
							break;
						}
						else if (pBlueWarrior->isAlive() && !pRedWarrior->isAlive()){
							cout<<setfill('0')<<setw(3)<<time_<<":40 blue "<<pBlueWarrior->kind<<' '<<pBlueWarrior->id<<" killed red "<<
							pRedWarrior->kind<<' '<<pRedWarrior->id<<" in city "<<cityID<<" remaining "<<pBlueWarrior->hp<<" elements"<<endl;
							//去掉死了的warrior
							pRedWarrior->nowCity = n+2;
							break;
						}
						else{ //both alive
							if (!pRedWarrior->getWeaponCnt() && !pBlueWarrior->getWeaponCnt()){
								//000:40 both red iceman 1 and blue lion 12 were alive in city 2
								cout<<setfill('0')<<setw(3)<<time_<<":40 both red "<<pRedWarrior->kind<<' '<<pRedWarrior->id<<" and blue "<<
								pBlueWarrior->kind<<' '<<pBlueWarrior->id<<" were alive in city "<<cityID<<endl;
							break;
							}
						}
					}
					if (pBlueWarrior->getWeaponCnt()){
						//攻击
						int bAd = pBlueWarrior->pWeapons[bCurWeapon]->getAd(pBlueWarrior->ad);
						pRedWarrior->takeDamage(bAd);
						if (pBlueWarrior->pWeapons[bCurWeapon]->kind == "bomb" && pBlueWarrior->kind != "ninja")
							pBlueWarrior->takeDamage(bAd / 2);
						//更新武器耐久度，去除废武器
						pBlueWarrior->pWeapons[bCurWeapon]->updateRemainTimes();
						pBlueWarrior->pWeapons.erase(
    						remove_if(pBlueWarrior->pWeapons.begin(), pBlueWarrior->pWeapons.end(),
                   			[](const Weapon* w) { return w->remainTimes == 0; }), pBlueWarrior->pWeapons.end());
						
						if (pBlueWarrior->getWeaponCnt())
							bCurWeapon = (bCurWeapon + 1) % pBlueWarrior->getWeaponCnt();
						//判断战斗结果
						if (pBlueWarrior->isAlive() && !pRedWarrior->isAlive()){
							//000:40 red iceman 1 killed blue lion 12 in city 2 remaining 20 elements
							cout<<setfill('0')<<setw(3)<<time_<<":40 blue "<<pBlueWarrior->kind<<' '<<pBlueWarrior->id<<" killed red "<<
							pRedWarrior->kind<<' '<<pRedWarrior->id<<" in city "<<cityID<<" remaining "<<pBlueWarrior->hp<<" elements"<<endl;
							//抢武器
							sort(pRedWarrior->pWeapons.begin(), pRedWarrior->pWeapons.end(), takeCmp);
							while (pBlueWarrior->pWeapons.size() < 10 && pRedWarrior->getWeaponCnt()){
								pBlueWarrior->pWeapons.push_back(pRedWarrior->pWeapons.back());
								pRedWarrior->pWeapons.pop_back();
							}
							//去掉死了的warrior
							pRedWarrior->nowCity = n+2;
							break;
						}
						else if (!pBlueWarrior->isAlive() && !pRedWarrior->isAlive()){
							//000:40 both red iceman 1 and blue lion 12 died in city 2
							cout<<setfill('0')<<setw(3)<<time_<<":40 both red "<<pRedWarrior->kind<<' '<<pRedWarrior->id<<" and blue "<<
							pBlueWarrior->kind<<' '<<pBlueWarrior->id<<" died in city "<<cityID<<endl;
							//去掉死了的warrior
							pRedWarrior->nowCity = n+2;
							pBlueWarrior->nowCity = -1;
							break;
						}
						else if (!pBlueWarrior->isAlive() && pRedWarrior->isAlive()){
							cout<<setfill('0')<<setw(3)<<time_<<":40 red "<<pRedWarrior->kind<<' '<<pRedWarrior->id<<" killed blue "<<
							pBlueWarrior->kind<<' '<<pBlueWarrior->id<<" in city "<<cityID<<" remaining "<<pRedWarrior->hp<<" elements"<<endl;
							//去掉死了的warrior
							pBlueWarrior->nowCity = -1;
							break;
						}
						else{ //both alive
							if (!pRedWarrior->getWeaponCnt() && !pBlueWarrior->getWeaponCnt()){
								//000:40 both red iceman 1 and blue lion 12 were alive in city 2
								cout<<setfill('0')<<setw(3)<<time_<<":40 both red "<<pRedWarrior->kind<<' '<<pRedWarrior->id<<" and blue "<<
								pBlueWarrior->kind<<' '<<pBlueWarrior->id<<" were alive in city "<<cityID<<endl;
								break;
							}
						}
					}
				}
				else{
					if (pBlueWarrior->getWeaponCnt()){
						//攻击
						int bAd = pBlueWarrior->pWeapons[bCurWeapon]->getAd(pBlueWarrior->ad);
						pRedWarrior->takeDamage(bAd);
						if (pBlueWarrior->pWeapons[bCurWeapon]->kind == "bomb" && pBlueWarrior->kind != "ninja")
							pBlueWarrior->takeDamage(bAd / 2);
						//更新武器耐久度，去除废武器
						pBlueWarrior->pWeapons[bCurWeapon]->updateRemainTimes();
						pBlueWarrior->pWeapons.erase(
    						remove_if(pBlueWarrior->pWeapons.begin(), pBlueWarrior->pWeapons.end(),
                   			[](const Weapon* w) { return w->remainTimes == 0; }), pBlueWarrior->pWeapons.end());
						
						if (pBlueWarrior->getWeaponCnt())
							bCurWeapon = (bCurWeapon + 1) % pBlueWarrior->getWeaponCnt();
						//判断战斗结果
						if (pBlueWarrior->isAlive() && !pRedWarrior->isAlive()){
							//000:40 red iceman 1 killed blue lion 12 in city 2 remaining 20 elements
							cout<<setfill('0')<<setw(3)<<time_<<":40 blue "<<pBlueWarrior->kind<<' '<<pBlueWarrior->id<<" killed red "<<
							pRedWarrior->kind<<' '<<pRedWarrior->id<<" in city "<<cityID<<" remaining "<<pBlueWarrior->hp<<" elements"<<endl;
							//抢武器
							sort(pRedWarrior->pWeapons.begin(), pRedWarrior->pWeapons.end(), takeCmp);
							while (pBlueWarrior->pWeapons.size() < 10 && pRedWarrior->getWeaponCnt()){
								pBlueWarrior->pWeapons.push_back(pRedWarrior->pWeapons.back());
								pRedWarrior->pWeapons.pop_back();
							}
							//去掉死了的warrior
							pRedWarrior->nowCity = n+2;
							break;
						}
						else if (!pBlueWarrior->isAlive() && !pRedWarrior->isAlive()){
							//000:40 both red iceman 1 and blue lion 12 died in city 2
							cout<<setfill('0')<<setw(3)<<time_<<":40 both red "<<pRedWarrior->kind<<' '<<pRedWarrior->id<<" and blue "<<
							pBlueWarrior->kind<<' '<<pBlueWarrior->id<<" died in city "<<cityID<<endl;
							//去掉死了的warrior
							pRedWarrior->nowCity = n+2;
							pBlueWarrior->nowCity = -1;
							break;
						}
						else if (!pBlueWarrior->isAlive() && pRedWarrior->isAlive()){
							cout<<setfill('0')<<setw(3)<<time_<<":40 red "<<pRedWarrior->kind<<' '<<pRedWarrior->id<<" killed blue "<<
							pBlueWarrior->kind<<' '<<pBlueWarrior->id<<" in city "<<cityID<<" remaining "<<pRedWarrior->hp<<" elements"<<endl;
							//去掉死了的warrior
							pBlueWarrior->nowCity = -1;
							break;
						}
						else{ //both alive
							if (!pRedWarrior->getWeaponCnt() && !pBlueWarrior->getWeaponCnt()){
								//000:40 both red iceman 1 and blue lion 12 were alive in city 2
								cout<<setfill('0')<<setw(3)<<time_<<":40 both red "<<pRedWarrior->kind<<' '<<pRedWarrior->id<<" and blue "<<
								pBlueWarrior->kind<<' '<<pBlueWarrior->id<<" were alive in city "<<cityID<<endl;
								break;
							}
						}
					}
					if (pRedWarrior->getWeaponCnt()){
						//攻击
						int aAd = pRedWarrior->pWeapons[aCurWeapon]->getAd(pRedWarrior->ad);
						pBlueWarrior->takeDamage(aAd);
						if (pRedWarrior->pWeapons[aCurWeapon]->kind == "bomb" && pRedWarrior->kind != "ninja")
							pRedWarrior->takeDamage(aAd / 2);
						//更新武器耐久度，去除废武器
						pRedWarrior->pWeapons[aCurWeapon]->updateRemainTimes();
						pRedWarrior->pWeapons.erase(
    						remove_if(pRedWarrior->pWeapons.begin(), pRedWarrior->pWeapons.end(),
                   			[](const Weapon* w) { return w->remainTimes == 0; }), pRedWarrior->pWeapons.end());

						if (pRedWarrior->getWeaponCnt())
							aCurWeapon = (aCurWeapon + 1) % pRedWarrior->getWeaponCnt();
						//判断战斗结果
						if (!pBlueWarrior->isAlive() && pRedWarrior->isAlive()){
							//000:40 red iceman 1 killed blue lion 12 in city 2 remaining 20 elements
							cout<<setfill('0')<<setw(3)<<time_<<":40 red "<<pRedWarrior->kind<<' '<<pRedWarrior->id<<" killed blue "<<
							pBlueWarrior->kind<<' '<<pBlueWarrior->id<<" in city "<<cityID<<" remaining "<<pRedWarrior->hp<<" elements"<<endl;
							//抢武器
							sort(pBlueWarrior->pWeapons.begin(), pBlueWarrior->pWeapons.end(), takeCmp);
							while (pRedWarrior->pWeapons.size() < 10 && pBlueWarrior->getWeaponCnt()){
								pRedWarrior->pWeapons.push_back(pBlueWarrior->pWeapons.back());
								pBlueWarrior->pWeapons.pop_back();
							}
							//去掉死了的warrior
							pBlueWarrior->nowCity = -1;
							break;
						}
						else if (!pBlueWarrior->isAlive() && !pRedWarrior->isAlive()){
							//000:40 both red iceman 1 and blue lion 12 died in city 2
							cout<<setfill('0')<<setw(3)<<time_<<":40 both red "<<pRedWarrior->kind<<' '<<pRedWarrior->id<<" and blue "<<
							pBlueWarrior->kind<<' '<<pBlueWarrior->id<<" died in city "<<cityID<<endl;
							//去掉死了的warrior
							pRedWarrior->nowCity = n+2;
							pBlueWarrior->nowCity = -1;
							break;
						}
						else if (pBlueWarrior->isAlive() && !pRedWarrior->isAlive()){
							cout<<setfill('0')<<setw(3)<<time_<<":40 blue "<<pBlueWarrior->kind<<' '<<pBlueWarrior->id<<" killed red "<<
							pRedWarrior->kind<<' '<<pRedWarrior->id<<" in city "<<cityID<<" remaining "<<pBlueWarrior->hp<<" elements"<<endl;
							//去掉死了的warrior
							pRedWarrior->nowCity = n+2;
							break;
						}
						else{ //both alive
							if (!pRedWarrior->getWeaponCnt() && !pBlueWarrior->getWeaponCnt()){
								//000:40 both red iceman 1 and blue lion 12 were alive in city 2
								cout<<setfill('0')<<setw(3)<<time_<<":40 both red "<<pRedWarrior->kind<<' '<<pRedWarrior->id<<" and blue "<<
								pBlueWarrior->kind<<' '<<pBlueWarrior->id<<" were alive in city "<<cityID<<endl;
								break;
							}
						}
					}
				}
				//不应该判断武器指针相等，应该判断武器的状态
				//而武器的状态不再变化不只是用完一轮后不变化，因为可能还有一些可消耗的武器没用
				//所以武器状态不再变化其实只有两种可能，只剩下sword或没有武器
				bool equal1 = aPreviousPWeapons.size() == pRedWarrior->pWeapons.size()
					&& equal(aPreviousPWeapons.begin(), aPreviousPWeapons.end(), pRedWarrior->pWeapons.begin(), ptrCmp);
				bool equal2 = bPreviousPWeapons.size() == pBlueWarrior->pWeapons.size()
					&& equal(bPreviousPWeapons.begin(), bPreviousPWeapons.end(), pBlueWarrior->pWeapons.begin(), ptrCmp);
				bool equal3 = isAllSwords(pRedWarrior->pWeapons) && isAllSwords(pBlueWarrior->pWeapons);
				if (aPreviousHp == pRedWarrior->hp && bPreviousHp == pBlueWarrior->hp && equal1 && equal2 &&equal3){
					//000:40 both red iceman 1 and blue lion 12 were alive in city 2
					cout<<setfill('0')<<setw(3)<<time_<<":40 both red "<<pRedWarrior->kind<<' '<<pRedWarrior->id<<" and blue "<<
					pBlueWarrior->kind<<' '<<pBlueWarrior->id<<" were alive in city "<<cityID<<endl;
					for (auto pw : aPreviousPWeapons)
    					delete pw;
					for (auto pw : bPreviousPWeapons)
    					delete pw;
					break;
				}
				aPreviousHp = pRedWarrior->hp;
				bPreviousHp = pBlueWarrior->hp;
				for (auto pw : aPreviousPWeapons)
    				delete pw;
				aPreviousPWeapons.clear();
				for (auto pw : bPreviousPWeapons)
    				delete pw;
				bPreviousPWeapons.clear();
				for (auto& pw : pRedWarrior->pWeapons){
    				aPreviousPWeapons.push_back(pw->clone());
				}
				for (auto& pw : pBlueWarrior->pWeapons){
    				bPreviousPWeapons.push_back(pw->clone());
				}
			}
		}
	}
	void City::dragonYell(const int& time_){
		if (pRedWarrior && pBlueWarrior){
			if (pRedWarrior->isAlive() && pRedWarrior->kind == "dragon"){
				//003:40 blue dragon 2 yelled in city 4
				cout<<setfill('0')<<setw(3)<<time_<<":40 red dragon "<<pRedWarrior->id<<" yelled in city "<<cityID<<endl;
			}
			if (pBlueWarrior->isAlive() && pBlueWarrior->kind == "dragon"){
				//003:40 blue dragon 2 yelled in city 4
				cout<<setfill('0')<<setw(3)<<time_<<":40 blue dragon "<<pBlueWarrior->id<<" yelled in city "<<cityID<<endl;
			}
		}	
	}
	void City::reportStatus(const int& time_){
		if (pRedWarrior){
			if (pRedWarrior->isAlive())
			//000:55 blue wolf 2 has 2 sword 3 bomb 0 arrow and 7 elements
				cout<<setfill('0')<<setw(3)<<time_<<":55 red "<<pRedWarrior->kind<<' '<<pRedWarrior->id<<" has "
				<<pRedWarrior->getEachWeaponCnt("sword")<<" sword "<<pRedWarrior->getEachWeaponCnt("bomb")<<" bomb "
				<<pRedWarrior->getEachWeaponCnt("arrow")<<" arrow and "<<pRedWarrior->hp<<" elements"<<endl;
		}
		if (pBlueWarrior){
			if (pBlueWarrior->isAlive())
				cout<<setfill('0')<<setw(3)<<time_<<":55 blue "<<pBlueWarrior->kind<<' '<<pBlueWarrior->id<<" has "
				<<pBlueWarrior->getEachWeaponCnt("sword")<<" sword "<<pBlueWarrior->getEachWeaponCnt("bomb")<<" bomb "
				<<pBlueWarrior->getEachWeaponCnt("arrow")<<" arrow and "<<pBlueWarrior->hp<<" elements"<<endl;
		}
	}

//-------------------------------------------------------------
int main(){
	int testCases;
	cin>>testCases;
	int tt = testCases;
	for (;testCases > 0;--testCases){
		cout<<"Case "<<tt-testCases+1<<":"<<endl;
		cin>>m>>n>>k>>t;
		for(int i = 0;i < warriorNum;++i){
			int tmp;
			cin>>tmp;
			Warrior::hps[i] = tmp;
		}
		for(int i = 0;i < warriorNum;++i){
			int tmp;
			cin>>tmp;
			Warrior::ads[i] = tmp;
		}
		Headquarter rHeadquarter("red", m);
		Headquarter bHeadquarter("blue", m);
		int time = 0;
		int quotient = t / 60;
		int remainder = t % 60;
		vector<bool> breakers(7, false);
		if (0 <= remainder && remainder < 5)
			breakers[0] = true;
		else if (5 <= remainder && remainder < 10)
			breakers[1] = true;
		else if (10 <= remainder && remainder < 35)
			breakers[2] = true;
		else if (35 <= remainder && remainder < 40)
			breakers[3] = true;
		else if (40 <= remainder && remainder < 50)
			breakers[4] = true;
		else if (50 <= remainder && remainder < 55)
			breakers[5] = true;
		else if (55 <= remainder && remainder <60)
			breakers[6] = true;
		while (!rHeadquarter.isTaken and !bHeadquarter.isTaken && time <= quotient){
			vector<City> Cities(n+2);
			for (int i = 0;i < n+2;++i)
				Cities[i].cityID = i;
			//00
			rHeadquarter.produce(time);
			bHeadquarter.produce(time);
			if (time == quotient && breakers[0])
				break;
			//05
			lionRun(time, rHeadquarter, bHeadquarter);
			if (time == quotient && breakers[1])
				break;
			//10
			rHeadquarter.march(time); //march不是先红后蓝，是自西向东
			bHeadquarter.march(time);
			updateCities(rHeadquarter, bHeadquarter, Cities);
			
			if (Cities[0].pBlueWarrior){
				rHeadquarter.isTaken = true;
				cout<<setfill('0')<<setw(3)<<time<<":10 blue "<<Cities[0].pBlueWarrior->kind<<' '<<Cities[0].pBlueWarrior->id
				<<" reached red headquarter with "<<Cities[0].pBlueWarrior->hp<<" elements and force "<<Cities[0].pBlueWarrior->ad<<endl
				<<setfill('0')<<setw(3)<<time<<":10 red headquarter was taken"<<endl;
			}
			
			reportMarch(time, rHeadquarter, bHeadquarter, Cities);

			if (Cities[n+1].pRedWarrior){
				bHeadquarter.isTaken = true;
				//001:10 red iceman 1 reached blue headquarter with 20 elements and force 30
				//003:10 blue headquarter was taken
				cout<<setfill('0')<<setw(3)<<time<<":10 red "<<Cities[n+1].pRedWarrior->kind<<' '<<Cities[n+1].pRedWarrior->id
				<<" reached blue headquarter with "<<Cities[n+1].pRedWarrior->hp<<" elements and force "<<Cities[n+1].pRedWarrior->ad<<endl
				<<setfill('0')<<setw(3)<<time<<":10 blue headquarter was taken"<<endl;
			}
			
			if (time == quotient && breakers[2])
				break;
			if (!rHeadquarter.isTaken and !bHeadquarter.isTaken){
				for (auto& c : Cities){
					//35
					c.wolfTake(time);
				}
				if (time == quotient && breakers[3])
					break;
				for (auto& c : Cities){
					//40
					c.fightAndReport(time);
					c.dragonYell(time);
				}
				if (time == quotient && breakers[4])
					break;
				//50
				rHeadquarter.reportHp(time);
				bHeadquarter.reportHp(time);
				if (time == quotient && breakers[5])
					break;
				for (auto& c : Cities){
					//55
					c.reportStatus(time);
				}
			}
		++time;
		}
	}
	return 0;
}
