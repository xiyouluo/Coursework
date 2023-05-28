#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

const int warriorNum = 5, rewardElements = 8, cityElementsAddNum = 10;

int m, n, r, k, t;

class Warrior;
class Headquarter;

class Weapon {
	public:
		string kind; // 武器种类
		int id; // 武器序号
		int weaponAd; // 武器攻击力
		bool illegal = false; // 是否是废武器
		int remainTimes = 0x3f3f3f3f; // 剩余使用次数

		Weapon(int id_): id(id_) {
			if (id_ == 0) kind = "sword";
			else if (id_ == 1) kind = "bomb";
			else if (id_ == 2) kind = "arrow";
		}

		virtual ~Weapon() = default;
		virtual void updateWeapon() {}
		virtual string getInfo() const = 0;
};

class Sword: public Weapon {
	public:
		Sword(Warrior* pw);
		~Sword() = default;

		void updateWeapon() override {
			weaponAd = weaponAd * 8 / 10;
			if (weaponAd <= 0) illegal = true;
		}

		virtual string getInfo() const override {
			return "(" + to_string(weaponAd) + ")";
		}
};

class Bomb: public Weapon {
	public:
		Bomb(): Weapon(1) {}
		~Bomb() = default;

		virtual string getInfo() const override {
			return string();
		}
};

class Arrow: public Weapon {
	public:
		Arrow(): Weapon(2) {
			weaponAd = r;
			remainTimes = 3;
		}

		~Arrow() = default;

		virtual void updateWeapon() override {
			-- remainTimes;
			if (remainTimes <= 0) illegal = true;
		}

		virtual string getInfo() const override {
			return "(" + to_string(remainTimes) + ")";
		}
};


class City {
	friend class Warrior;

	public:
		Warrior* pRedWarrior = nullptr;
		Warrior* pBlueWarrior = nullptr;
		int cityID;
		string flag = "none"; //red, blue, none
		string lastWinner = "none"; //red, blue, none
		int elements = 0;

		City() {}
		~City() = default;
		void tpElements(const int& time_);
		void useBomb(const int& time_);
		void fightAndProcess(const int& time_);
		void reportStatus(const int& time_, const char* color);
};


class Warrior {
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
		vector<Weapon*> pWeapons;
		bool shouldBeRewarded = false;
		bool dieOfArrow = false;

		static vector<string> kinds; //种类列表（实现种类和数字间的双射）
		static vector<int> hps; //生成战士需要的hp列表
		static vector<int> ads; //战士初始的攻击力列表
		
		Warrior(Headquarter* pH, int kindNo);

		virtual ~Warrior() {
			for (auto p : pWeapons) delete p;
			pWeapons.clear();
		}

		inline bool isAlive() {
			return hp > 0;
		}

		virtual void attack(Warrior* pEnemy) {
			int weaponAd = 0;
			for (auto pWeapon : pWeapons)
				if (pWeapon->kind == "sword") {
					weaponAd += pWeapon->weaponAd;
					pWeapon->updateWeapon();
					break;
				}
					
			pEnemy->hp -= ad + weaponAd;
		}

		virtual void counterAttack(Warrior* pEnemy) {
			int weaponAd = 0;
			for (auto pWeapon : pWeapons)
				if (pWeapon->kind == "sword") {
					weaponAd += pWeapon->weaponAd;
					pWeapon->updateWeapon();
					break;
				}
			
			pEnemy->hp -= ad / 2 + weaponAd;
		}

		bool willDie(Warrior* pEnemy, bool isActive) {
			int enemyWeaponAd = 0;
			for (auto pWeapon : pEnemy->pWeapons)
				if (pWeapon->kind == "sword") {
					enemyWeaponAd += pWeapon->weaponAd;
					break;
				}

			if (isActive) {
				if (pEnemy->kind == "ninja") return false;

				int weaponAd = 0;
				for (auto pWeapon : pWeapons)
					if (pWeapon->kind == "sword") {
						weaponAd += pWeapon->weaponAd;
						break;
					}
				
				if (pEnemy->hp > ad + weaponAd && hp <= pEnemy->ad / 2 + enemyWeaponAd) return true;
			}
			
			else if (hp <= pEnemy->ad + enemyWeaponAd) return true;
			
			return false;
		}

		void removeIllegalWeapons() {
			pWeapons.erase(remove_if(pWeapons.begin(), pWeapons.end(), [](Weapon* pWeapon) {
				if (pWeapon->illegal == true) {
					delete pWeapon;
					return true;
				}
				return false;
			}), pWeapons.end());
		}

		void getElements(City* pCity);
		void reportWeapons(const int& time_);
};

class Dragon: public Warrior {
	public:
		double morale;
		bool activeAttacked = false; //主动攻击过
		
		Dragon(Headquarter* pH, int kindNo);
		~Dragon() = default;

		virtual void attack(Warrior* pEnemy) override {
			activeAttacked = true;

			int weaponAd = 0;
			for (auto pWeapon : pWeapons)
				if (pWeapon->kind == "sword") {
					weaponAd += pWeapon->weaponAd;
					pWeapon->updateWeapon();
					break;
				}
					
			pEnemy->hp -= ad + weaponAd;
		}

		void yell(const int& time_);
};

class Ninja: public Warrior {
	public:
		Ninja(Headquarter* pH, int kindNo): Warrior(pH, kindNo) {
			if (id % 3 == 0) {
				pWeapons.push_back(new Sword(this));
				removeIllegalWeapons();
			}
			else if (id % 3 == 1) pWeapons.push_back(new Bomb());
			else pWeapons.push_back(new Arrow());
	

			if ((id + 1) % 3 == 0) {
				pWeapons.push_back(new Sword(this));
				removeIllegalWeapons();
			}
			else if ((id + 1) % 3 == 1) pWeapons.push_back(new Bomb());
			else pWeapons.push_back(new Arrow());
		}

		~Ninja() = default;

		virtual void counterAttack(Warrior* pEnemy) override {
			return;
		}
};

class Iceman: public Warrior {
	public:
		bool movedOneStep = false;

		Iceman(Headquarter* pH, int kindNo): Warrior(pH, kindNo) {
			if (id % 3 == 0) {
				pWeapons.push_back(new Sword(this));
				removeIllegalWeapons();
			}
			else if (id % 3 == 1) pWeapons.push_back(new Bomb());
			else pWeapons.push_back(new Arrow());
		}

		~Iceman() = default;
};

class Lion: public Warrior {
	public:
		int loyalty;
		int lastHp;

		Lion(Headquarter* pH, int kindNo);
		~Lion() = default;
};

class Wolf: public Warrior {
	private:
		bool hasWeapons[3] = {false}; // 0: sword, 1: bomb, 2: arrow
	public:
		Wolf(Headquarter* pH, int kindNo): Warrior(pH, kindNo) {}
		~Wolf() = default;

		void robWeapons(Warrior* pEnemy);
};


class Headquarter {
	friend class Warrior;
	friend class Dragon;
	friend class Lion;
	friend class City;
	friend void updateCities(Headquarter& r, Headquarter& b, vector<City>& C);

	private: 
		vector<int> order; //生成武士的顺序列表，武士用一个序号表示
		int totalNum; //武士总数
		vector<int> nums; //各类武士的数量列表
		int totalHp; //阵营当前所剩的总hp
		int curMakingIdx; //当前制造序号
	
	public:
		string color;
		bool isTaken;
		vector<Warrior*> pWarriors; //装指针，用多态！！！
		vector<Warrior*> pEnemyWarriors;
		
		Headquarter(string color_, int totalHp_): color(color_), totalHp(totalHp_) {
			if (color_ == "red") order = {2, 3, 4, 1, 0};
			else if (color_ == "blue") order = {3, 0, 1, 2, 4};
			
			totalNum = 0;
			nums = {0, 0, 0, 0, 0};
			curMakingIdx = 0;
			isTaken = false;
		}

		~Headquarter() {
			for (auto pw : pWarriors) delete pw;
			pWarriors.clear();
		}

		void produce(int time_);
		void march(int time_);
		void reportHp(int time_);
		void rewardWarriors(const vector<City>& cities, int time_);
};


Sword::Sword(Warrior* pw): Weapon(0) {
	weaponAd = pw->ad * 2 / 10;
	if (weaponAd <= 0) illegal = true; // 有些sword生成时就是0攻击力
}

Warrior::Warrior(Headquarter* pH, int kindNo) {
	kind = kinds[kindNo];
	pHeadquarter = pH;
	hp = hps[kindNo];
	ad = ads[kindNo];
	id = ++pH->totalNum;
	++pH->nums[kindNo];
	pH->totalHp -= hp;

	// 输出:00 blue lion 5 born
	cout << ":00 " << pH->color << ' ' << kinds[kindNo] << ' ' << id << " born" << endl;

	if (pH->color == "red") {
		nowCity = 0;
	}

	else if (pH->color == "blue") {
		nowCity = n + 1;
	}
}

Dragon::Dragon(Headquarter* pH, int kindNo): Warrior(pH, kindNo) {
	morale = (double)pH->totalHp / (double)hp;
	cout << "Its morale is " << fixed << setprecision(2) << morale << endl;
	
	if (id % 3 == 0) {
		pWeapons.push_back(new Sword(this));
		removeIllegalWeapons();
	}
	else if (id % 3 == 1) pWeapons.push_back(new Bomb());
	else pWeapons.push_back(new Arrow());
}

void Dragon::yell(const int& time_) {
	// 003:40 blue dragon 2 yelled in city 
	if (activeAttacked && morale > 0.8)
		cout << setfill('0') << setw(3) << time_ << ":40 " << pHeadquarter->color << " dragon " << id 
			<< " yelled in city " << nowCity << endl;
}

Lion::Lion(Headquarter* pH, int kindNo): Warrior(pH, kindNo) {
	lastHp = hp;
	loyalty = pH->totalHp;
	cout << "Its loyalty is " << loyalty << endl;
}

void Warrior::getElements(City* pCity) {
	pHeadquarter->totalHp += pCity->elements;
	pCity->elements = 0;
}

void Warrior::reportWeapons(const int& time_) {
	// 000:55 blue wolf 2 has arrow(2),bomb,sword(23)
	// 000:55 blue wolf 4 has no weapon
	// 000:55 blue wolf 5 has sword(20)
	if (pWeapons.size() == 0)
		cout << setfill('0') << setw(3) << time_ << ":55 " << pHeadquarter->color << " " << kind << " " << id 
			<< " has no weapon" << endl;
	
	else if (pWeapons.size() == 1)
		cout << setfill('0') << setw(3) << time_ << ":55 " << pHeadquarter->color << " " << kind << " " << id 
			<< " has " << pWeapons[0]->kind << pWeapons[0]->getInfo() << endl;

	else {
		sort(pWeapons.begin(), pWeapons.end(), [](Weapon* p1, Weapon* p2) {return p1->kind < p2->kind;});
		if (pWeapons.size() == 2) {
			cout << setfill('0') << setw(3) << time_ << ":55 " << pHeadquarter->color << " " << kind << " " << id 
				<< " has " << pWeapons[0]->kind << pWeapons[0]->getInfo() << "," << pWeapons[1]->kind << pWeapons[1]->getInfo() << endl;
		}

		else {
			cout << setfill('0') << setw(3) << time_ << ":55 " << pHeadquarter->color << " " << kind << " " << id 
				<< " has " << pWeapons[0]->kind << pWeapons[0]->getInfo() << "," << pWeapons[1]->kind << pWeapons[1]->getInfo() << "," 
				<< pWeapons[2]->kind << pWeapons[2]->getInfo() << endl;
		}
	}

}

void Wolf::robWeapons(Warrior* pEnemy) {
	for (auto pWeapon : pEnemy->pWeapons)
		if (!hasWeapons[pWeapon->id]) {
			pWeapons.push_back(pWeapon);
			hasWeapons[pWeapon->id] = true;
			pEnemy->pWeapons.erase(find(pEnemy->pWeapons.begin(), pEnemy->pWeapons.end(), pWeapon));
		}
}

void Headquarter::produce(int time_) {
	if (totalHp < Warrior::hps[order[curMakingIdx]]) return;

	cout << setfill('0') << setw(3) << time_; 
	if (order[curMakingIdx] == 0) pWarriors.push_back(new Dragon(this, 0));
	else if (order[curMakingIdx] == 1) pWarriors.push_back(new Ninja(this, 1));
	else if (order[curMakingIdx] == 2) pWarriors.push_back(new Iceman(this, 2));
	else if (order[curMakingIdx] == 3) pWarriors.push_back(new Lion(this, 3));
	else if (order[curMakingIdx] == 4) pWarriors.push_back(new Wolf(this, 4));

	curMakingIdx = (curMakingIdx + 1) % warriorNum;
}

void Headquarter::march(int time_) {
	for (auto pWarrior : pWarriors) {
		if (color == "red") ++ pWarrior->nowCity;
		else if (color == "blue") -- pWarrior->nowCity;

		if (pWarrior->kind == "dragon") {
			Dragon* pDragon = dynamic_cast<Dragon*>(pWarrior);
			pDragon->activeAttacked = false;
		}

		else if (pWarrior->kind == "iceman") {
			bool isAlive = pWarrior->isAlive();
			Iceman* pIceman = dynamic_cast<Iceman*>(pWarrior);			
			if (pIceman->movedOneStep == true) {
				pIceman->hp -= 9;
				if (pIceman->hp <= 0 && isAlive) pIceman->hp = 1;
				pIceman->ad += 20;
				pIceman->movedOneStep = false;
			}

			else pIceman->movedOneStep = true;
		}

		else if (pWarrior->kind == "lion") {
			Lion* pLion = dynamic_cast<Lion*>(pWarrior);
			pLion->lastHp = pLion->hp;
		}

	}
	
	pWarriors.erase(remove_if(pWarriors.begin(), pWarriors.end(), [this](Warrior* pWarrior) {
		if (!pWarrior->isAlive()) {
			delete pWarrior;
			return true;
		}
		return false;
	}), pWarriors.end());
}

void Headquarter::reportHp(int time_) {
	//000:50 100 elements in red headquarter
	cout<<setfill('0')<<setw(3)<<time_<<":50 "<<totalHp<<" elements in "<<color<<" headquarter"<<endl;
}

void Headquarter::rewardWarriors(const vector<City>& cities, int time_) {
	if (color == "red") {
		for (int i = n; i >= 1; -- i) {
			auto pWarrior = cities[i].pRedWarrior;
            if (pWarrior == nullptr) continue;

			if (totalHp < 8) pWarrior->shouldBeRewarded = false;			
			else if (pWarrior->shouldBeRewarded) {
				pWarrior->hp += rewardElements;
				totalHp -= rewardElements;
				pWarrior->shouldBeRewarded = false;
			}
		}
	}

	else {
        for (int i = 1; i <= n; ++ i) {
            auto pWarrior = cities[i].pBlueWarrior;
            if (pWarrior == nullptr) continue;

            if (totalHp < 8) pWarrior->shouldBeRewarded = false;			
            else if (pWarrior->shouldBeRewarded) {
                pWarrior->hp += rewardElements;
                totalHp -= rewardElements;
                pWarrior->shouldBeRewarded = false;
            }
        }
    }

}

void City::tpElements(const int& time_) {
	if (pRedWarrior && !pBlueWarrior && cityID <= n) {
		// 000:30 blue lion 1 earned 10 elements for his headquarter
		cout << setfill('0') << setw(3) << time_ << ":30 " << pRedWarrior->pHeadquarter->color << ' ' << pRedWarrior->kind << ' ' << pRedWarrior->id
			<< " earned " << elements << " elements for his headquarter" << endl;
		pRedWarrior->pHeadquarter->totalHp += elements;
		elements = 0;	
	}

	if (pBlueWarrior && !pRedWarrior && cityID >= 1) {
		// 000:30 blue lion 1 earned 10 elements for his headquarter
		cout << setfill('0') << setw(3) << time_ << ":30 " << pBlueWarrior->pHeadquarter->color << ' ' << pBlueWarrior->kind << ' ' << pBlueWarrior->id
			<< " earned " << elements << " elements for his headquarter" << endl;
		pBlueWarrior->pHeadquarter->totalHp += elements;
		elements = 0;
	}

}

void City::useBomb(const int& time_) {
	if (!pRedWarrior || !pBlueWarrior ||!pRedWarrior->isAlive() || !pBlueWarrior->isAlive()) return;

	for (auto pWeapon : pRedWarrior->pWeapons) {
		if (pWeapon->kind == "bomb") {
			bool isActive = (flag == "red" || (flag == "none" && cityID % 2 == 1));
			if (pRedWarrior->willDie(pBlueWarrior, isActive)) {
				// 000:38 blue dragon 1 used a bomb and killed red lion 7
				cout << setfill('0') << setw(3) << time_ << ":38 red " << pRedWarrior->kind << ' ' << pRedWarrior->id 
					<< " used a bomb and killed blue " << pBlueWarrior->kind << ' ' << pBlueWarrior->id << endl;
				pRedWarrior->hp = 0;
				pRedWarrior = nullptr;
				pBlueWarrior->hp = 0;
				pBlueWarrior = nullptr;
				return;
			}	
		}
	}

	for (auto pWeapon : pBlueWarrior->pWeapons) {
		if (pWeapon->kind == "bomb") {
			bool isActive = (flag == "blue" || (flag == "none" && cityID % 2 == 0));
			if (pBlueWarrior->willDie(pRedWarrior, isActive)) {
				// 000:38 blue dragon 1 used a bomb and killed red lion 7
				cout << setfill('0') << setw(3) << time_ << ":38 blue " << pBlueWarrior->kind << ' ' << pBlueWarrior->id 
					<< " used a bomb and killed red " << pRedWarrior->kind << ' ' << pRedWarrior->id << endl;
				pRedWarrior->hp = 0;
				pRedWarrior = nullptr;
				pBlueWarrior->hp = 0;
				pBlueWarrior = nullptr;
				return;
			}			
		}
	}

}

void City::fightAndProcess(const int& time_) {
	if (!pRedWarrior || !pBlueWarrior) {
		// 处理一方被箭射死，另一方此城市没人的情况
		if (pRedWarrior && !pRedWarrior->isAlive()) pRedWarrior = nullptr;
		if (pBlueWarrior && !pBlueWarrior->isAlive()) pBlueWarrior = nullptr;
		
		return;
	}

	// 处理dragon的对手被队友射了的情况
	if (pRedWarrior->kind == "dragon" && !pBlueWarrior->isAlive() && (flag == "red" || (flag == "none" && cityID % 2 == 1))) {
		Dragon* pDragon = dynamic_cast<Dragon*>(pRedWarrior);
		pDragon->activeAttacked = true;
	}
	if (pBlueWarrior->kind == "dragon" && !pRedWarrior->isAlive() && (flag == "blue" || (flag == "none" && cityID % 2 == 0))) {
		Dragon* pDragon = dynamic_cast<Dragon*>(pBlueWarrior);
		pDragon->activeAttacked = true;
	}

	// fight
	if (pRedWarrior->isAlive() && pBlueWarrior->isAlive()) {
		if (flag == "red" || (flag == "none" && cityID % 2 == 1)) {
			pRedWarrior->attack(pBlueWarrior);
			// 000:40 red iceman 1 attacked blue lion 1 in city 1 with 20 elements and force 30
			cout << setfill('0') << setw(3) << time_ << ":40 red " << pRedWarrior->kind << ' ' << pRedWarrior->id 
				<< " attacked blue " << pBlueWarrior->kind << ' ' << pBlueWarrior->id << " in city " << cityID 
				<< " with " << pRedWarrior->hp << " elements and force " << pRedWarrior->ad << endl;
		}

		else if (flag == "blue" || (flag == "none" && cityID % 2 == 0)) {
			pBlueWarrior->attack(pRedWarrior);
			// 000:40 red iceman 1 attacked blue lion 1 in city 1 with 20 elements and force 30
			cout << setfill('0') << setw(3) << time_ << ":40 blue " << pBlueWarrior->kind << ' ' << pBlueWarrior->id 
				<< " attacked red " << pRedWarrior->kind << ' ' << pRedWarrior->id << " in city " << cityID 
				<< " with " << pBlueWarrior->hp << " elements and force " << pBlueWarrior->ad << endl;
		}

	}

	// fight back
	if (pRedWarrior->isAlive() && pBlueWarrior->isAlive()) {
		if (flag == "red" || (flag == "none" && cityID % 2 == 1)) {
			pBlueWarrior->counterAttack(pRedWarrior);
			// 001:40 blue dragon 2 fought back against red lion 2 in city 1
			if (pBlueWarrior->kind != "ninja")
				cout << setfill('0') << setw(3) << time_ << ":40 blue " << pBlueWarrior->kind << ' ' << pBlueWarrior->id 
					<< " fought back against red " << pRedWarrior->kind << ' ' << pRedWarrior->id << " in city " << cityID << endl;
		}

		else if (flag == "blue" || (flag == "none" && cityID % 2 == 0)) {
			pRedWarrior->counterAttack(pBlueWarrior);
			// 001:40 blue dragon 2 fought back against red lion 2 in city 1
			if (pRedWarrior->kind != "ninja")
				cout << setfill('0') << setw(3) << time_ << ":40 red " << pRedWarrior->kind << ' ' << pRedWarrior->id 
					<< " fought back against blue " << pBlueWarrior->kind << ' ' << pBlueWarrior->id << " in city " << cityID << endl;
		}

	}

	if (pRedWarrior->isAlive() && !pBlueWarrior->isAlive()) pRedWarrior->shouldBeRewarded = true;
	if (pBlueWarrior->isAlive() && !pRedWarrior->isAlive()) pBlueWarrior->shouldBeRewarded = true;

	// 处理战斗结果
	if (pRedWarrior->isAlive() && !pBlueWarrior->isAlive()) {
		pRedWarrior->removeIllegalWeapons();
		// 001:40 red lion 2 was killed in city 1
		if (!pBlueWarrior->dieOfArrow)
			cout << setfill('0') << setw(3) << time_ << ":40 blue " << pBlueWarrior->kind << ' ' << pBlueWarrior->id 
				<< " was killed in city " << cityID << endl;

		if (pRedWarrior->kind == "dragon") {
			Dragon* pDragon = dynamic_cast<Dragon*>(pRedWarrior);
			pDragon->morale += 0.2;
			pDragon->yell(time_);
		}
		else if (pRedWarrior->kind == "wolf") {
			Wolf* pWolf = dynamic_cast<Wolf*>(pRedWarrior);
			pWolf->robWeapons(pBlueWarrior);
		}

		if (pBlueWarrior->kind == "lion") {
			Lion* pLion = dynamic_cast<Lion*>(pBlueWarrior);
			pRedWarrior->hp += max(0, pLion->lastHp);
		}

		if (elements > 0) {
			// 001:40 blue dragon 2 earned 10 elements for his headquarter
			cout << setfill('0') << setw(3) << time_ << ":40 red " << pRedWarrior->kind << ' ' << pRedWarrior->id 
				<< " earned " << elements << " elements for his headquarter" << endl;
			pRedWarrior->getElements(this);
		}

		if (lastWinner == "red" && flag != "red") {
			flag = "red";
			// 004:40 blue flag raised in city 4
			cout << setfill('0') << setw(3) << time_ << ":40 red flag raised in city " << cityID << endl;
		}

		lastWinner = "red";
		pBlueWarrior = nullptr;
	}

	else if (pBlueWarrior->isAlive() && !pRedWarrior->isAlive()) {
		pBlueWarrior->removeIllegalWeapons();
		// 001:40 red lion 2 was killed in city 1
		if (!pRedWarrior->dieOfArrow)
			cout << setfill('0') << setw(3) << time_ << ":40 red " << pRedWarrior->kind << ' ' << pRedWarrior->id 
				<< " was killed in city " << cityID << endl;

		if (pBlueWarrior->kind == "dragon") {
			Dragon* pDragon = dynamic_cast<Dragon*>(pBlueWarrior);
			pDragon->morale += 0.2;
			pDragon->yell(time_);
		}
		else if (pBlueWarrior->kind == "wolf") {
			Wolf* pWolf = dynamic_cast<Wolf*>(pBlueWarrior);
			pWolf->robWeapons(pRedWarrior);
		}

		if (pRedWarrior->kind == "lion") {
			Lion* pLion = dynamic_cast<Lion*>(pRedWarrior);
			pBlueWarrior->hp += max(0, pLion->lastHp);
		}

		if (elements > 0) {
			// 001:40 blue dragon 2 earned 10 elements for his headquarter
			cout << setfill('0') << setw(3) << time_ << ":40 blue " << pBlueWarrior->kind << ' ' << pBlueWarrior->id 
				<< " earned " << elements << " elements for his headquarter" << endl;
			pBlueWarrior->getElements(this);
		}
			
		if (lastWinner == "blue" && flag != "blue") {
			flag = "blue";
			// 004:40 blue flag raised in city 4
			cout << setfill('0') << setw(3) << time_ << ":40 blue flag raised in city " << cityID << endl;
		}

		lastWinner = "blue";
		pRedWarrior = nullptr;
	}

	else if (pRedWarrior->isAlive() && pBlueWarrior->isAlive()) {
		pRedWarrior->removeIllegalWeapons();
		pBlueWarrior->removeIllegalWeapons();

		if (pRedWarrior->kind == "dragon") {
			Dragon* pDragon = dynamic_cast<Dragon*>(pRedWarrior);
			pDragon->morale -= 0.2;
			pDragon->yell(time_);
		}
		else if (pRedWarrior->kind == "lion") {
			Lion* pLion = dynamic_cast<Lion*>(pRedWarrior);
			pLion->loyalty -= k;
		}

		if (pBlueWarrior->kind == "dragon") {
			Dragon* pDragon = dynamic_cast<Dragon*>(pBlueWarrior);
			pDragon->morale -= 0.2;
			pDragon->yell(time_);
		}		
		else if (pBlueWarrior->kind == "lion") {
			Lion* pLion = dynamic_cast<Lion*>(pBlueWarrior);
			pLion->loyalty -= k;
		}		
	
		lastWinner = "none";
	}

	else {
		pRedWarrior	= nullptr;
		pBlueWarrior = nullptr;
		// lastWinner = "none"; 
		// 这句不能写，因为都死了就只可能是都被另一个人用箭射死，而这种情况下题目需要保持lastWinner不变，因为没有发生符合题目描述的“战斗”
		// （炸死的已经被处理成空指针抹除了）
	}

}

void City::reportStatus(const int& time_, const char* color) {
	if (color == "red" && pRedWarrior && pRedWarrior->nowCity < n + 1) pRedWarrior->reportWeapons(time_);
	else if (color == "blue" && pBlueWarrior && pBlueWarrior->nowCity > 0) pBlueWarrior->reportWeapons(time_);
}

void lionRun(int time_, Headquarter& r, Headquarter& b) {
	sort(r.pWarriors.begin(), r.pWarriors.end(), [](Warrior* w1, Warrior* w2) { return w1->nowCity < w2->nowCity; }); //西边的排前面
	sort(b.pWarriors.begin(), b.pWarriors.end(), [](Warrior* w1, Warrior* w2) { return w1->nowCity < w2->nowCity; });

	for (auto pw : r.pWarriors) {
		if (pw->nowCity <= n && pw->kind == "lion") {
			Lion* pLion = dynamic_cast<Lion*>(pw);
			if (pLion->loyalty <= 0) {
				// 000:05 blue lion 1 ran away
				cout << setfill('0') << setw(3) << time_ << ":05 red lion " << pw->id << " ran away" << endl;
				delete pw;
				r.pWarriors.erase(find(r.pWarriors.begin(), r.pWarriors.end(), pw));
			}
		}
	}
	
	for (auto pw : b.pWarriors) {
		if (pw->nowCity >= 1 && pw->kind == "lion") {
			Lion* pLion = dynamic_cast<Lion*>(pw);
			if (pLion->loyalty <= 0) {
				// 000:05 blue lion 1 ran away
				cout << setfill('0') << setw(3) << time_ << ":05 blue lion " << pw->id << " ran away" << endl;
				delete pw;
				b.pWarriors.erase(find(b.pWarriors.begin(), b.pWarriors.end(), pw));
			}
		}
	}
}

void updateCities(Headquarter& r, Headquarter& b, vector<City>& C) {
    for (City& city : C) {
        city.pRedWarrior = nullptr;
        city.pBlueWarrior = nullptr;
    }

    for (auto pw : r.pWarriors) {
		if (pw->nowCity <= n + 1) C[pw->nowCity].pRedWarrior = pw;
		if (pw->nowCity == n + 1) b.pEnemyWarriors.push_back(pw);
	}
          
    for (auto pw : b.pWarriors) {
		if (pw->nowCity >= 0) C[pw->nowCity].pBlueWarrior = pw;
		if (pw->nowCity == 0) r.pEnemyWarriors.push_back(pw);
	}

}

void arriveEnemyHeadquarter(Warrior* pw, Headquarter& take, Headquarter& beenTaken, int time) {
	//001:10 red iceman 1 reached blue headquarter with 20 elements and force 30
	cout << setfill('0') << setw(3) << time << ":10 " << take.color << ' ' << pw->kind << ' ' << pw->id << " reached " << beenTaken.color 
		<< " headquarter with " << pw->hp << " elements and force " << pw->ad << endl;
	
	if (beenTaken.pEnemyWarriors.size() >= 2) {
		beenTaken.isTaken = true;
		//003:10 blue headquarter was taken
		cout << setfill('0') << setw(3) << time << ":10 " << beenTaken.color << " headquarter was taken" << endl;
	}			
	
}

void reportMarch(vector<City>& cities, const int& time_) {
	for (auto& c : cities) {
		if (c.pRedWarrior && c.pRedWarrior->nowCity < n + 1)
				cout << setfill('0') << setw(3) << time_ << ":10 red " << c.pRedWarrior->kind << ' ' << c.pRedWarrior->id << " marched to city " 
				<< c.pRedWarrior->nowCity << " with " << c.pRedWarrior->hp << " elements and force " << c.pRedWarrior->ad << endl;
			
		if (c.pBlueWarrior && c.pBlueWarrior->nowCity > 0)
				cout << setfill('0') << setw(3) << time_ << ":10 blue " << c.pBlueWarrior->kind << ' ' << c.pBlueWarrior->id << " marched to city " 
				<< c.pBlueWarrior->nowCity << " with " << c.pBlueWarrior->hp << " elements and force " << c.pBlueWarrior->ad << endl;
	}
}

void useArrow(const vector<City>& cities, const int& time_) {
	for (int i = 0; i <= n; ++ i) {
		if (cities[i].pRedWarrior && i <= n - 1 && cities[i + 1].pBlueWarrior) {
			Warrior* pRedWarrior = cities[i].pRedWarrior;
			Warrior* pBlueWarrior = cities[i + 1].pBlueWarrior;
			for (auto pWeapon : pRedWarrior->pWeapons) {
				if (pWeapon->kind == "arrow") {
					pBlueWarrior->hp -= pWeapon->weaponAd;
					pWeapon->updateWeapon();
					pRedWarrior->removeIllegalWeapons();
					if (pBlueWarrior->kind == "lion") {
						Lion* pLion = dynamic_cast<Lion*>(pBlueWarrior);
						pLion->lastHp = pLion->hp;
					}

					// 000:35 blue dragon 1 shot
					cout << setfill('0') << setw(3) << time_ << ":35 red " << pRedWarrior->kind << ' ' << pRedWarrior->id << " shot";
					if (!pBlueWarrior->isAlive()) {
						// 000:35 blue dragon 1 shot and killed red lion 4
						cout << " and killed blue " << pBlueWarrior->kind << ' ' << pBlueWarrior->id << endl;
						pBlueWarrior->hp = 0;
						pBlueWarrior->dieOfArrow = true;
					}
					else cout << endl;
					break;
				}
			}
		}

		if (cities[i].pBlueWarrior && i >= 1 && cities[i - 1].pRedWarrior) {
			Warrior* pBlueWarrior = cities[i].pBlueWarrior;
			Warrior* pRedWarrior = cities[i - 1].pRedWarrior;
			for (auto pWeapon : pBlueWarrior->pWeapons) {
				if (pWeapon->kind == "arrow") {
					pRedWarrior->hp -= pWeapon->weaponAd;
					pWeapon->updateWeapon();
					pBlueWarrior->removeIllegalWeapons();
					if (pRedWarrior->kind == "lion") {
						Lion* pLion = dynamic_cast<Lion*>(pRedWarrior);
						pLion->lastHp = pLion->hp;
					}

					// 000:35 blue dragon 1 shot
					cout << setfill('0') << setw(3) << time_ << ":35 blue " << pBlueWarrior->kind << ' ' << pBlueWarrior->id << " shot";
					if (!pRedWarrior->isAlive()) {
						// 000:35 blue dragon 1 shot and killed red lion 4
						cout << " and killed red " << pRedWarrior->kind << ' ' << pRedWarrior->id << endl;
						pRedWarrior->hp = 0;
						pRedWarrior->dieOfArrow = true;
					}
					else cout << endl;
					break;
				}
			}
		}

	}	
}

inline int classify(int x) {
    if (x >= 0 && x < 5) return 0;
    if (x >= 5 && x < 10) return 1;
    if (x >= 10 && x < 20) return 2;
    if (x >= 20 && x < 30) return 3;
    if (x >= 30 && x < 35) return 4;
    if (x >= 35 && x < 38) return 5;
    if (x >= 38 && x < 40) return 6;
    if (x >= 40 && x < 50) return 7;
    if (x >= 50 && x < 55) return 8;
    if (x >= 55 && x < 60) return 9;
	else throw "error: invalid input for function classify";
}

vector<string> Warrior::kinds = {"dragon", "ninja", "iceman", "lion", "wolf"};
vector<int> Warrior::hps(warriorNum);
vector<int> Warrior::ads(warriorNum);

int main() {
	// freopen("in.txt", "r", stdin);
	// freopen("output.txt", "w", stdout);
	int totalCases;
	cin >> totalCases;

	for (int caseNo = 1; caseNo <= totalCases; caseNo ++ ) {
		cin >> m >> n >> r >> k >> t;
		for(int i = 0; i < warriorNum; ++ i) cin >> Warrior::hps[i];
		for(int i = 0; i < warriorNum; ++ i) cin >> Warrior::ads[i];

		cout << "Case " << caseNo << ":" << endl;
		
		Headquarter rHeadquarter("red", m);
		Headquarter bHeadquarter("blue", m);

		int time = 0;
		int quotient = t / 60; // 总时间整除60的商
		int remainder = t % 60; // 总时间模60
		bool breakers[10] = {false}; // 用来控制输出停止
		breakers[classify(remainder)] = true;
		
		vector<City> Cities(n + 2);
		for (int i = 0;i < n + 2; ++ i) Cities[i].cityID = i;

		while (true) {
			// 00
			rHeadquarter.produce(time);
			bHeadquarter.produce(time);
			if (time == quotient && breakers[0]) break;

			// 05
			lionRun(time, rHeadquarter, bHeadquarter);
			if (time == quotient && breakers[1]) break;

			// 10
			rHeadquarter.march(time);
			bHeadquarter.march(time);
			updateCities(rHeadquarter, bHeadquarter, Cities);

			if (Cities[0].pBlueWarrior) arriveEnemyHeadquarter(Cities[0].pBlueWarrior, bHeadquarter, rHeadquarter, time);
			reportMarch(Cities, time);
			if (Cities[n + 1].pRedWarrior) arriveEnemyHeadquarter(Cities[n + 1].pRedWarrior, rHeadquarter, bHeadquarter, time);

			if (rHeadquarter.isTaken || bHeadquarter.isTaken) break;
			if (time == quotient && breakers[2]) break;

			// 20
			for (auto& c : Cities) c.elements += cityElementsAddNum;
			if (time == quotient && breakers[3]) break;

			// 30
			for (auto& c : Cities) c.tpElements(time); // teleport
			if (time == quotient && breakers[4]) break;
			
			// 35
			useArrow(Cities, time);
			if (time == quotient && breakers[5]) break;

			// 38
			for (auto& c : Cities) c.useBomb(time);
			if (time == quotient && breakers[6]) break;

			// 40
			for (auto& c : Cities) c.fightAndProcess(time);
			rHeadquarter.rewardWarriors(Cities, time);
			bHeadquarter.rewardWarriors(Cities, time);
			if (time == quotient && breakers[7]) break;

			// 50
			rHeadquarter.reportHp(time);
			bHeadquarter.reportHp(time);
			if (time == quotient && breakers[8]) break;

			// 55
			for (auto& c : Cities) c.reportStatus(time, "red");
			if (!bHeadquarter.pEnemyWarriors.empty()) bHeadquarter.pEnemyWarriors[0]->reportWeapons(time);
			
			if (!rHeadquarter.pEnemyWarriors.empty()) rHeadquarter.pEnemyWarriors[0]->reportWeapons(time);
			for (auto& c : Cities) c.reportStatus(time, "blue");
			
			++ time;
		}
	}
	
	return 0;
}
