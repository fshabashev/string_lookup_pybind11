#include <iostream>
#include <math.h>
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>


double pi_integral() {
    double sum = 0.0;
    long n = 100*1000*1000;
    for (long k = 0; k < n; k++){
        double x = ((double) k)/n;
        double val = sqrt(1.0 - x*x);
        sum += val;
    }
    return 4.0*sum/n;
}

double bbp_pi_calculation_no_multiplier(long k) {
    double k_f64 = (double) k;
    double val = 4.0/(8.0*k_f64+1.0) - 2.0/(8.0*k_f64 + 4.0) - 1.0/(8.0*k_f64+5.0) - 1.0/(8.0*k_f64+6.0);
    return val;
}

double bbp_pi_calcuation(long k){
    double val = bbp_pi_calculation_no_multiplier(k);
    double k_f64 = (double) k;
    double multiplier = 1.0/(pow(16, k_f64));
    return val*multiplier;
}

void calculate_pi_bbp() {
    double sum = 0.0;
    for (long i = 0; i < 100; i++){
        sum += bbp_pi_calcuation(i);
    }
    std::cout << sum << std::endl;
}

std::string gen_random(const int len) {
    static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return tmp_s;
}

std::vector<std::string> gen_rand_string_vector(long N){
    std::vector<std::string> string_vec;
    int P = 10;
    for (int i = 0; i<N; i++){
        string_vec.push_back(gen_random(P));
    }
    return string_vec;

}

// sort vector of pairs
template <typename T>
void sort_vector_of_pairs(std::vector<std::pair<T, T>> &vec) {
    std::sort(vec.begin(), vec.end(),
              [](const std::pair<T, T> &a, const std::pair<T, T> &b) {
                  return a.first < b.first;
              });
}

std::vector<std::pair<std::string, std::string> > gen_random_pairs(long N){
    std::vector<std::pair<std::string, std::string> > string_vec;
    int P = 10;
    for (int i = 0; i<N; i++){
        string_vec.push_back(std::make_pair(gen_random(P), gen_random(P)));
    }
    return string_vec;
}

class StringPairHolder {
public:
    std::vector<std::pair<std::string, std::string> > pair_storage;
    long N = 100*1000;
    StringPairHolder() {
        pair_storage = gen_random_pairs(N);
        sort_vector_of_pairs(pair_storage);
    }

    StringPairHolder(std::vector< std::pair<std::string, std::string> > &vec) {
        pair_storage = vec;
        sort_vector_of_pairs(pair_storage);
    }

    long binary_search(std::string key) {
        long left = 0;
        long right = pair_storage.size() - 1;
        while (left <= right) {
            long mid = (left + right) / 2;
            if (pair_storage[mid].first == key) {
                return mid;
            } else if (pair_storage[mid].first < key) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        return -1;
    }

    std::string get_value(std::string key) {
        long index = binary_search(key);
        if (index == -1) {
            return "";
        } else {
            return pair_storage[index].second;
        }
    }

    bool contains(std::string key) {
        return binary_search(key) != -1;
    }
};

class StringHolder {
public:
    std::vector<std::string> string_storage;
    long N = 100000;
    StringHolder(){
        this->string_storage = gen_rand_string_vector(this->N);
        std::sort(this->string_storage.begin(), this->string_storage.end());
    }

    StringHolder(std::vector<std::string> string_storage_param){
        this->string_storage = string_storage_param;
    }

    long get_index(std::string str){
        auto v = this->string_storage;
        auto val = str;
        auto lower = std::lower_bound(v.begin(), v.end(), val);
        const bool found = lower != v.end() && *lower == val;
        if (!found) {
            return -1;
        }
        auto idx = std::distance(v.begin(), lower);
        return idx;
    }

    bool lookup(std::string str) {
        return std::binary_search(this->string_storage.begin(), this->string_storage.end(), str);
    }
};

int main() {
    std::cout << "printing PI_   " << pi_integral() << std::endl;
    calculate_pi_bbp();

    auto str_storage = StringHolder();

    for (int i =0; i<str_storage.N; i++){
        std::cout << str_storage.string_storage[i];
        std::cout << "\n";
    }
    std::cout << str_storage.lookup(str_storage.string_storage[20]) << std::endl;
    std::cout << str_storage.lookup(std::string("asdfasdf")) << std::endl;


    auto str_pair_storage = StringPairHolder();
    std::cout << "test get_value " << str_pair_storage.get_value("asdf") << std::endl;
    std::cout << "test contains " <<  str_pair_storage.contains(std::string("asdf")) << std::endl;
    std::cout << "test get_value " << str_pair_storage.get_value(str_pair_storage.pair_storage[1].first) << std::endl;
    return 0;
}
namespace py = pybind11;

float some_fn (float arg1, float arg2) {
    return arg1 + arg2;
}

PYBIND11_MODULE(cpp_string_lookup, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring
    m.def("some_fn", &some_fn, "A function which adds two numbers");
    py::class_<StringHolder>(m, "StringHolder")
            .def(py::init< std::vector<std::string> >())
            .def("get_index", &StringHolder::get_index)
            .def("lookup", &StringHolder::lookup);
}

