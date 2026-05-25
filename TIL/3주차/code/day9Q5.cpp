#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

struct Movie {
    string title;
    double rating;
};

// TODO: MovieProcessor 추상 클래스 정의
// 순수 가상 함수 process를 선언해야 합니다.
// process는 vector<Movie>&를 인자로 받아야 합니다.
class MovieProcessor
{
public:
    virtual void process(vector<Movie>& movies) = 0;
    ~MovieProcessor() {};
};


// 기본 영화 관리자
class MovieManager {
private:
    vector<Movie> movies;
    map<string, double> movieMap;

public:
    MovieManager() {
        // 초기 데이터 설정
        movies = {
            {"Inception", 9.0},
            {"Interstellar", 8.6},
            {"The Dark Knight", 9.1},
            {"Memento", 8.4}
        };

        for (const auto& movie : movies) {
            movieMap[movie.title] = movie.rating;
        }
    }

    void printMovies() {
        cout << "영화 목록:\n";
        for (const auto& movie : movies) {
            cout << "제목: " << movie.title << ", 평점: " << movie.rating << "\n";
        }
    }

    void findMovie(const string& title) {
        auto it = movieMap.find(title);
        if (it != movieMap.end()) {
            cout << "영화 제목: " << it->first << ", 평점: " << it->second << "\n";
        }
        else {
            cout << "해당 영화는 목록에 없습니다.\n";
        }
    }

    // MovieProcessor를 사용하여 기능 확장
    void processMovies(MovieProcessor& processor) {
        processor.process(movies);
    }
};


// TODO: compareMovies 함수 정의
// Movies 객체의 대소를 비교하는 함수 입니다.
// STL에서 제공하는 sort 함수를 활용해서 vector<Movie>를 멤버변수 rating 기준 내림차순으로 정렬 할 수 있도록 해야 합니다. 
bool compareMovies(Movie& a, Movie& b) {
    return a.rating > b.rating;
};

// TODO: RatingSorter 클래스 정의
// MovieProcessor를 상속받아 구현합니다.
// process 는 vector<Movie>&를 인자로 받으며 영화목록이 저장되어 있습니다.
// process는 인자로 받은 벡터는 내림차순으로 정렬하고, 정렬된 영화목록을 출력합니다.
class RatingSorter : public MovieProcessor
{
public:
    void process(vector<Movie>& movies) override {
        sort(movies.begin(), movies.end(), compareMovies);
        cout << "평점 기준 정렬된 영화 목록:\n";
        for (const auto& movie : movies)
        {
            cout << "제목: " << movie.title << ", 평점: " << movie.rating << "\n";
        }
    };
    /*static bool compareMovies(Movie& a, Movie& b) {
        return a.rating > b.rating;
    };*/
};


// 구체 클래스: 특정 평점 이상의 영화 필터링
class RatingFilter : public MovieProcessor {
private:
    double minRating;

public:
    explicit RatingFilter(double minRating) : minRating(minRating) {}

    void process(vector<Movie>& movies) {
        cout << "평점 " << minRating << " 이상인 영화 목록:\n";
        for (const auto& movie : movies) {
            if (movie.rating >= minRating) {
                cout << "제목: " << movie.title << ", 평점: " << movie.rating << "\n";
            }
        }
    }
};

int main() {
    MovieManager manager;

    cout << "1. 영화 목록 출력\n";
    manager.printMovies();

    cout << "\n2. 영화 검색 (예: Interstellar)\n";
    manager.findMovie("Interstellar");

    cout << "\n3. 평점 기준 정렬 및 출력\n";
    RatingSorter sorter;
    manager.processMovies(sorter);

    cout << "\n4. 평점 8.5 이상인 영화 필터링 및 출력\n";
    RatingFilter filter(8.5);
    manager.processMovies(filter);

    return 0;
}

