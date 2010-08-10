require File.expand_path('../../spec_helper', __FILE__)

describe "Date#===" do

  it "should be true for the two same dates" do
    (Date.civil(2000, 04, 06) === Date.civil(2000, 04, 06)).should == true
  end

  it "should be true if comparison is a DateTime on the same date" do
    (Date.civil(2000, 04, 06) === DateTime.civil(2000, 04, 06, 10)).should == true
  end

  it "should be true if comparison is a Numeric with the same integer value as JD" do
    (Date.civil(2000, 04, 06) === Date.civil(2000, 04, 06).jd).should == true
  end
  
  it "should be false for different dates" do
    (Date.civil(2000, 04, 05) === Date.civil(2000, 04, 06)).should == false
  end
  
  it "should be false if comparison is a DateTime with a different date" do
    (Date.civil(2000, 04, 06) === DateTime.civil(2000, 04, 07, 10)).should == false
  end

  it "should be false if comparison is a Numeric with the different integer value as JD" do
    (Date.civil(2000, 04, 06) === Date.civil(2000, 04, 07).jd).should == false
  end
end
