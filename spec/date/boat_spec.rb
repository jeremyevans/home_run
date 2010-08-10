require File.expand_path('../../spec_helper', __FILE__)

describe "Date#<=>" do

  it "should be able to compare two same dates" do
    (Date.civil(2000, 04, 06) <=> Date.civil(2000, 04, 06)).should == 0
  end

  it "should be able to compute the difference between two dates" do
    (Date.civil(2000, 04, 05) <=> Date.civil(2000, 04, 06)).should == -1
    (Date.civil(2001, 04, 05) <=> Date.civil(2000, 04, 06)).should == 1
  end

  it "should be able to compare to another numeric" do
    (Date.civil(2000, 04, 05) <=> Date.civil(2000, 04, 06).jd).should == -1
    (Date.civil(2001, 04, 05) <=> Date.civil(2000, 04, 06).jd).should == 1
  end

end

describe "Date#between?" do
   it "should be true if the date falls in between the two given dates" do
    (Date.civil(2000, 04, 06).between?(Date.civil(2000, 04, 05), Date.civil(2000, 04, 07))).should == true
    (Date.civil(2000, 04, 06).between?(Date.civil(2000, 04, 06), Date.civil(2000, 04, 07))).should == true
    (Date.civil(2000, 04, 06).between?(Date.civil(2000, 04, 05), Date.civil(2000, 04, 06))).should == true
  end 
  
   it "should be false if the date does not fall in between the two given dates" do
    (Date.civil(2000, 04, 05).between?(Date.civil(2000, 04, 06), Date.civil(2000, 04, 07))).should == false
  end 
end