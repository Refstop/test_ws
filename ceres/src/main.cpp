#include "ceres_example/ceres_example.h"

// ceres::SizedCostFunction<1, 1> : residual 개수, 첫번째 파라미터 개수?
class QuadraticCostFunction : public ceres::SizedCostFunction<2, 2> {
    public:
    virtual ~QuadraticCostFunction() {}
    virtual bool Evaluate(double const* const* parameters, double* residuals, double** jacobians) const {
        const double x = parameters[0][0];
        const double y = parameters[0][1];
        // f(x) = 10 - x.
        residuals[0] = 10 - x;
        residuals[1] = 10 - 2*y;

        // f'(x) = -1. Since there's only 1 parameter and that parameter
        // has 1 dimension, there is only 1 element to fill in the
        // jacobians.
        //
        // Since the Evaluate function can be called with the jacobians
        // pointer equal to nullptr, the Evaluate function must check to see
        // if jacobians need to be computed.
        //
        // For this simple problem it is overkill to check if jacobians[0]
        // is nullptr, but in general when writing more complex
        // CostFunctions, it is possible that Ceres may only demand the
        // derivatives w.r.t. a subset of the parameter blocks.
        if (jacobians != nullptr && jacobians[0] != nullptr) {
            jacobians[0][0] = -1;
            jacobians[0][1] = 0;

            jacobians[0][2] = 0;
            jacobians[0][3] = -2;

            // jacobians[1][0] = 0;
            // jacobians[1][1] = -2;
        }
        return true;
    }
};

int main(int argc, char* argv[]) {
    google::InitGoogleLogging(argv[0]);

    // The variable to solve for with its initial value. It will be
    // mutated in place by the solver.
    double x = 0.5;
    const double initial_x = x;
    double y = 0.5;
    const double initial_y = y;
    double x_arr[2] = {x, y};
    // Build the problem.
    Problem problem;
    // Set up the only cost function (also known as residual).
    CostFunction* cost_function = new QuadraticCostFunction;
    problem.AddResidualBlock(cost_function, nullptr, x_arr);
    // Run the solver!
    Solver::Options options;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = true;
    Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);
    std::cout << summary.BriefReport() << "\n";
    std::cout << "x : " << initial_x << " -> " << x_arr[0] << "\n";
    std::cout << "y : " << initial_y << " -> " << x_arr[1] << "\n";
    return 0;
}