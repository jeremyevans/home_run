require File.expand_path('../../spec_helper', __FILE__)

describe "DateTime#hash" do
  it "should use the same hash value for equal datetimes" do 
    DateTime.civil(2004, 7, 12, 13, 14, 15).hash.should == DateTime.civil(2004, 7, 12, 13, 14, 15).hash
  end
  
  it "should use a different hash value for different dates" do 
    DateTime.civil(2004, 7, 12, 13, 14, 15).hash.should_not == DateTime.civil(2004, 7, 12, 13, 14, 16).hash
  end
end
