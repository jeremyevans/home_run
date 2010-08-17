require File.expand_path('../../spec_helper', __FILE__)

describe "DateTime#leap?" do
  it "should be true if the current date is a leap year and no argument is given" do
    DateTime.civil(2000, 10, 11).leap?.should ==  true
    DateTime.civil(2004, 10, 11).leap?.should ==  true
    DateTime.civil(2008, 10, 11).leap?.should ==  true
    DateTime.civil(1996, 10, 11).leap?.should ==  true
    DateTime.civil(1600, 10, 11).leap?.should ==  true
  end
  
  it "should be false if the current date is not a leap year and no argument is given" do
    DateTime.civil(1700, 10, 11).leap?.should ==  false
    DateTime.civil(1800, 10, 11).leap?.should ==  false
    DateTime.civil(1900, 10, 11).leap?.should ==  false
    DateTime.civil(1999, 10, 11).leap?.should ==  false
    DateTime.civil(2001, 10, 11).leap?.should ==  false
  end
end
