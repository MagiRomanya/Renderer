#ifndef UTILITIES_H_
#define UTILITIES_H_

template<typename T>
class SimpleTrigger {
    public:
        SimpleTrigger(const T start_value) : last_var(start_value) {}

        bool changed(T var) {
            if (last_var != var) {
                last_var = var;
                return true;
            }
            else {
                last_var = var;
                return false;
            }
        }
        bool changed_to_true(T var) {
            if ( (last_var != var) and var ) {
                last_var = var;
                return true;
            }
            else {
                last_var = var;
                return false;
            }
        }
    private:
        T last_var;
};

#endif // UTILITIES_H_
