#include <map>





class RegressionLine
{
    public:
		typedef std::map<double,double> Points;

        RegressionLine(Points & points);
        virtual ~RegressionLine();

        const double slope() const;
        const double yIntercept() const;
        const double regressionCoefficient() const;
    
    private:

        double slope_;
        double yIntercept_;
        double regressionCoefficient_;
    
};
