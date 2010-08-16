require File.expand_path('../../spec_helper', __FILE__)

describe "Date#hash" do
  it "should use the same hash value for equal dates" do 
    Date.civil(2004, 7, 12).hash.should == Date.civil(2004, 7, 12).hash
  end
  
  it "should use a different hash value for different dates" do 
    Date.civil(2004, 7, 12).hash.should_not == Date.civil(2004, 7, 13).hash
  end
end
