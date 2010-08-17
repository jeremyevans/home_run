require File.expand_path('../../spec_helper', __FILE__)

ruby_version_is "1.9" do
  describe "Date#sunday?" do
    it "should return true if the day is a Sunday" do
      Date.civil(2009,1,4).sunday?.should == true
    end

    it "should return false if the day is not a Sunday" do
      Date.civil(2009,1,2).sunday?.should == false
    end
  end 

  describe "Date#monday?" do
    it "should return true if the day is a Monday" do
      Date.civil(2009,1,5).monday?.should == true
    end

    it "should return false if the day is not a Monday" do
      Date.civil(2009,1,2).monday?.should == false
    end
  end 

  describe "Date#tuesday?" do
    it "should return true if the day is a Tuesday" do
      Date.civil(2009,1,6).tuesday?.should == true
    end

    it "should return false if the day is not a Tuesday" do
      Date.civil(2009,1,2).tuesday?.should == false
    end
  end 

  describe "Date#wednesday?" do
    it "should return true if the day is a Wednesday" do
      Date.civil(2009,1,7).wednesday?.should == true
    end

    it "should return false if the day is not a Tuesday" do
      Date.civil(2009,1,2).wednesday?.should == false
    end
  end 

  describe "Date#thursday?" do
    it "should return true if the day is a Thursday" do
      Date.civil(2009,1,1).thursday?.should == true
    end

    it "should return false if the day is not a Thursday" do
      Date.civil(2009,1,2).thursday?.should == false
    end
  end 

  describe "Date#friday?" do
    it "should return true if the day is a Friday" do
      Date.civil(2009,1,2).friday?.should == true
    end

    it "should return false if the day is not a Friday" do
      Date.civil(2009,1,1).friday?.should == false
    end
  end 

  describe "Date#saturday?" do
    it "should return true if the day is a Saturday" do
      Date.civil(2009,1,3).saturday?.should == true
    end

    it "should return false if the day is not a Saturday" do
      Date.civil(2009,1,2).saturday?.should == false
    end
  end 
end
