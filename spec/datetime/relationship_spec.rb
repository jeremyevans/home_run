require File.expand_path('../../spec_helper', __FILE__)

describe "DateTime#===" do

  it "should be true for the two same dates" do
    (DateTime.civil(2000, 04, 06) === DateTime.civil(2000, 04, 06)).should == true
  end

  it "should be true if comparison is a Date on the same date" do
    (DateTime.civil(2000, 04, 06, 10) === Date.civil(2000, 04, 06)).should == true
  end

  it "should be true if comparison is a Numeric with the same integer value as JD" do
    (DateTime.civil(2000, 04, 06) === DateTime.civil(2000, 04, 06).jd).should == true
  end
  
  it "should be false for different dates" do
    (DateTime.civil(2000, 04, 05) === DateTime.civil(2000, 04, 06)).should == false
  end
  
  it "should be false if comparison is a Date with a different date" do
    (DateTime.civil(2000, 04, 06, 10) === Date.civil(2000, 04, 07)).should == false
  end

  it "should be false if comparison is a Numeric with the different integer value as JD" do
    (DateTime.civil(2000, 04, 06) === DateTime.civil(2000, 04, 07).jd).should == false
  end
end
